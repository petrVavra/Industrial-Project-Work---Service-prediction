/*
    ModbusIP_ESP8266.cpp - Implementation for ModbusIP Library
    Copyright (C) 2014 Andr� Sarmento Barbosa
                  2017-2018 Alexander Emelianov (a.m.emelianov@gmail.com)
*/
#include "ModbusIP_ESP8266.h"

void ModbusIP::begin() {
	WiFiServer::begin();
	for (uint8_t i = 0; i < MODBUSIP_MAX_CLIENTS; i++)
		client[i] = NULL;
}

void ModbusIP::task() {
	uint8_t n, i;
	while (hasClient()) {
		WiFiClient* currentClient = new WiFiClient(available());
		if (currentClient != NULL && currentClient->connected()) {
			if (cbConnect == NULL || cbConnect(currentClient->remoteIP())) {
				for (n = 0; n < MODBUSIP_MAX_CLIENTS; n++) {
					if (client[n] == NULL) {
						client[n] = currentClient;
						break; // for
					}
				}
				if (n < MODBUSIP_MAX_CLIENTS)	// If client added process next
					continue; // while
			}
			// If callback returns false or _MAX_CLIENTS reached immediate close connection
			currentClient->flush();
			currentClient->stop();
			delete currentClient;
		}
	}
	for (n = 0; n < MODBUSIP_MAX_CLIENTS; n++) {
		if (client[n] == NULL)
			continue;
		if (!client[n]->connected()) {
			delete client[n];
			client[n] = NULL;
			continue;
		}
		uint16_t raw_len = 0;
		raw_len = client[n]->available();
		if (raw_len > 7) {
			for (i = 0; i < 7; i++)	_MBAP[i] = client[n]->read(); //Get MBAP

			_len = _MBAP[4] << 8 | _MBAP[5];
			_len--; // Do not count with last byte from MBAP
			if (_MBAP[2] != 0 || _MBAP[3] != 0) continue;   //Not a MODBUSIP packet
			if (_len > MODBUSIP_MAXFRAME) continue;      //Length is over MODBUSIP_MAXFRAME
			_frame = (uint8_t*) malloc(_len);
			
			raw_len = raw_len - 7;
			for (i = 0; i < _len; i++)	_frame[i] = client[n]->read(); //Get Modbus PDU
			
			this->receivePDU(_frame);
			client[n]->flush();

			if (_reply != MB_REPLY_OFF) {
			    //MBAP
				_MBAP[4] = (_len+1) >> 8;     //_len+1 for last byte from MBAP
				_MBAP[5] = (_len+1) & 0x00FF;
				
				size_t send_len = (uint16_t)_len + 7;
				uint8_t sbuf[send_len];
				
				for (i = 0; i < 7; i++)	    sbuf[i] = _MBAP[i];
				for (i = 0; i < _len; i++)	sbuf[i+7] = _frame[i];

				client[n]->write(sbuf, send_len);
			}
			free(_frame);
			_len = 0;
		}
	}
}

void ModbusIP::onConnect(cbModbusConnect cb = NULL) {
	cbConnect = cb;
}