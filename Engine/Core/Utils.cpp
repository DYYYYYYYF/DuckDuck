#include "Utils.hpp"
#include "Platform/File.hpp"

#include "Resources/Shader.hpp"

std::vector<uint32_t> Utils::CompileShader(const std::string& file, enum ShaderStage shaderStage, bool writeToDisk/* = true*/) {
	shaderc_shader_kind scShadercStage;
	switch (shaderStage)
	{
	case ShaderStage::eShader_Stage_Vertex:
		scShadercStage = shaderc_shader_kind::shaderc_vertex_shader;
		break;
	case ShaderStage::eShader_Stage_Fragment:
		scShadercStage = shaderc_shader_kind::shaderc_fragment_shader;
		break;
	}

	File ShaderSource(file);
	std::string Content = ShaderSource.ReadBytes();
	shaderc::Compiler compiler;
	shaderc::CompileOptions options;
	options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_3);
	options.SetTargetSpirv(shaderc_spirv_version_1_6);
	options.SetOptimizationLevel(shaderc_optimization_level_performance);	// 优化：性能优先

	// Like -DMY_DEFINE=1
	//options.AddMacroDefinition("MY_DEFINE", "1");

	shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(Content, scShadercStage, file.c_str(), options);

	if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
		LOG_ERROR("Compile shader %s failed.\n\
			Error msg: %s",
			file.c_str(),
			module.GetErrorMessage().c_str()
		);
		return std::vector<uint32_t>();
	}

	if (writeToDisk) {
		File OutFile(file);

		if (!OutFile.IsExist()) {

		}
		
	}

	return std::vector<uint32_t>(module.cbegin(), module.cend());
}