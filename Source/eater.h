/*
File: eater.h
Author: Starman0620
*/

#pragma once
#include <iostream>
#include <fstream>
#include <map>
#include <bitset>
#include <filesystem>
#include <chrono>
#include <thread>

void ReadROM();
uint8_t MemoryRead(uint16_t address);
void MemoryWrite(uint16_t address, uint8_t value);