#include "MaterialBase.h"

MaterialBase::MaterialBase(const std::string& shaderFileName, const int cbFloat4Size)
	:
	shaderHandle_(-1),
	constBufHandle_(-1),
	filePath_(shaderFileName),
	constBufFloat4Size_(cbFloat4Size),
	constBufParams_{}
{
}

MaterialBase::~MaterialBase(void)
{
	DeleteShader(shaderHandle_);
	DeleteShaderConstantBuffer(constBufHandle_);

	constBufParams_.clear();
}

void MaterialBase::Release(void)
{
	DeleteShader(shaderHandle_);
	DeleteShaderConstantBuffer(constBufHandle_);

	constBufParams_.clear();
}

void MaterialBase::AddConstBuf(const FLOAT4& cbParam)
{
	if (constBufFloat4Size_ > constBufParams_.size())
	{
		constBufParams_.emplace_back(cbParam);
	}
}

void MaterialBase::AddConstBuf(const VECTOR& valA, const float& valB)
{
	FLOAT4 ret = { valA.x,valA.y,valA.z,valB };

	AddConstBuf(ret);
}

void MaterialBase::SetConstBuf(const int idx, const FLOAT4& cbParam)
{
	if (idx >= constBufParams_.size())
	{
		return;
	}

	constBufParams_[idx] = cbParam;
}

void MaterialBase::SetConstBuf(const int idx, const VECTOR& valA, const float& valB)
{
	FLOAT4 ret = { valA.x,valA.y,valA.z,valB };

	SetConstBuf(idx,ret);
}

void MaterialBase::CreateConstBuf(void)
{
	constBufHandle_ =
		CreateShaderConstantBuffer(sizeof(FLOAT4) * constBufFloat4Size_);
}
