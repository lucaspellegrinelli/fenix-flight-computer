#ifndef SENSOR_FALSO_H
#define SENSOR_FALSO_H

#include "arduino_consts.h"

#include <fstream>
#include <string>
#include <vector>
#include <iostream>

class SensorFalso{
public:
	SensorFalso(std::string arquivo_leitura_caminho, int refresh_clock_ms){
		arquivo_leitura.open(arquivo_leitura_caminho);
		this->refresh_clock_ms = refresh_clock_ms;
	}

protected:
	std::ifstream arquivo_leitura;
	int refresh_clock_ms;
	int last_read_i = 0;

	bool ler_sensor(float saida[]){
		if(current_ms / refresh_clock_ms > last_read_i){
			for(std::string line; getline(arquivo_leitura, line);){
				std::vector<float> tokens;
				size_t pos = 0;
				std::string token;

				while ((pos = line.find(" ")) != std::string::npos) {
			    token = line.substr(0, pos);
			    tokens.push_back(std::stof(token));
			    line.erase(0, pos + 1);
				}

				if(tokens[0] >= current_ms){ // tokens[0] -> ms of the reading
					std::copy(tokens.begin(), tokens.end(), saida);
					last_read_i = current_ms / refresh_clock_ms;
					break;
				}
			}

			return true;
		}else{
			return false;
		}
	}
};

#endif
