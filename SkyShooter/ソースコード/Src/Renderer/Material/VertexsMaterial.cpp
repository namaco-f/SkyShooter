
#include "../../Application.h"
#include "VertexsMaterial.h"

VertexsMaterial::VertexsMaterial(const std::string& shaderFileName,const int cbSize)
	: 
	MaterialBase((Application::PATH_SHADER + shaderFileName), cbSize)
{
}

VertexsMaterial::~VertexsMaterial(void)
{
}

void VertexsMaterial::LoadShader(const std::string& shaderFileName)
{	
	shaderHandle_ = 
		LoadVertexShader(
			(Application::PATH_SHADER + shaderFileName).c_str()
		);
}

void VertexsMaterial::LoadShader(void)
{
	shaderHandle_ =
		LoadVertexShader(filePath_.c_str());
	CreateConstBuf();
}
