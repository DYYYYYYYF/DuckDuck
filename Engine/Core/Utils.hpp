#pragma once

#include <string>
#include <vector>

// Shader compiler
#include <shaderc/shaderc.hpp>

class Utils {
public:
	static std::vector<uint32_t> CompileShader(const std::string& file, enum ShaderStage shaderStage, bool writeToDisk = true);
};

