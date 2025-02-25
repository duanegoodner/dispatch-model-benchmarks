#pragma once

#include <chrono>
#include <iostream>
#include <string>

// External declaration for preventing compiler optimizations
extern volatile double prevent_optimization;

// Utility function to print elapsed time
void PrintTime(const std::string& label, std::chrono::duration<double> elapsed);