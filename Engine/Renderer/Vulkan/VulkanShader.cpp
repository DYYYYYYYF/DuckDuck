#include "VulkanShader.hpp"
#include "Systems/ResourceSystem.h"
#include "Core/EngineLogger.hpp"
#include "Platform/File.hpp"

std::vector<uint32_t> VulkanShader::CompileShaderFile(const char* filename, shaderc_shader_kind shadercStage, bool writeToDisk) {
	std::string SourceName(filename);
	size_t PrePathIndex = SourceName.find_first_of('/');
	size_t SufPathIndex = SourceName.find_last_of(".");
	std::string PrePath = SourceName.substr(0, PrePathIndex);
	std::string SufPath = SourceName.substr(PrePathIndex, SufPathIndex - PrePathIndex);
	std::string ShaderSourceFilename;
	switch (Type)
	{
	case ShaderType::eGLSL:
		ShaderSourceFilename = "../" + PrePath + "/glsl" + SufPath;
		break;
	case ShaderType::eHLSL:
		ShaderSourceFilename = "../" + PrePath + "/hlsl" + SufPath;
		break;
	default:
		ShaderSourceFilename = "../" + PrePath + "/glsl" + SufPath;
		break;
	}

	LOG_WARN("Tring to recompile shader file %s...", SufPath.c_str());

	File ShaderSource(ShaderSourceFilename);
	std::string Content = ShaderSource.ReadBytes();
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
	options.SetTargetSpirv(shaderc_spirv_version_1_6);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);	// 优化

	// Like -DMY_DEFINE=1
	//options.AddMacroDefinition("MY_DEFINE", "1");

	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(Content, shadercStage, filename, options);

	if (module.GetCompilationStatus() !=
		shaderc_compilation_status_success) {
		LOG_ERROR("Compile shader %s failed.\n\
			Error msg: %s",
			filename,
			module.GetErrorMessage().c_str()
		);
	}

	std::vector<uint32_t> SPRIV = std::vector<uint32_t>(module.cbegin(), module.cend());

	// 写入文件
	if (writeToDisk) {
		std::string SPRIVFilePath = ResourceSystem::GetRootPath() + std::string("/") + std::string(filename);
		File OutFile(SPRIVFilePath);
		OutFile.WriteBytes(reinterpret_cast<const char*>(SPRIV.data()), SPRIV.size() * sizeof(uint32_t), std::ios::trunc | std::ios::binary);
	}

	return SPRIV;
}
