

#include "../Application.h"

#include "../Renderer/Material/PixelMaterial.h"
#include "../Renderer/Material/VertexsMaterial.h"
#include "../Renderer/Renderer.h"

#include "ShaderResource.h"

using namespace std;


ShaderResource::ShaderResource(
	const std::string& psFileName,
	const int& psCbSizeFloat4,
	const std::string& vsFileName,
	const int& vsCbSizeFloat4
)
	:
	psMaterial_(make_shared<PixelMaterial>(psFileName, psCbSizeFloat4)),
	vsMaterial_(make_shared<VertexsMaterial>(vsFileName, vsCbSizeFloat4)),
	renderer_(nullptr)
{
}

ShaderResource::ShaderResource(const std::string& psFileName, const int& psCbSizeFloat4)
	:
	psMaterial_(make_shared<PixelMaterial>(psFileName, psCbSizeFloat4)),
	vsMaterial_(nullptr),
	renderer_(nullptr)
{
}

ShaderResource::~ShaderResource(void)
{
}

void ShaderResource::Release(void)
{
	psMaterial_.reset();

	if (IsUsedVS())
	{
		vsMaterial_.reset();
	}

	renderer_.reset();
}

void ShaderResource::Load(void)
{
	psMaterial_->LoadShader();

	if (!IsUsedVS()) { return; }
	
	vsMaterial_->LoadShader();
}

const bool ShaderResource::IsUsedVS(void) const
{
	if (vsMaterial_ == nullptr)
	{
		//頂点シェーダーを使用していない
		return false;
	}

	return true;
}

void ShaderResource::AddConstBuf_VS(const FLOAT4& val)
{
	if (!vsMaterial_) { return; }

	vsMaterial_->AddConstBuf(val);
}

void ShaderResource::AddConstBuf_PS(const FLOAT4& val)
{
	if (!psMaterial_) { return; }

	psMaterial_->AddConstBuf(val);
}

std::weak_ptr<PixelMaterial> ShaderResource::GetPS(void) const
{
	return psMaterial_;
}

std::weak_ptr<VertexsMaterial> ShaderResource::GetVS(void) const
{
	return vsMaterial_;
}

std::weak_ptr<Renderer> ShaderResource::GetRenderer(void)
{
	if (renderer_ == nullptr)
	{
		//レンダラークラスが作成されてない場合、作成
		MakeRenderer();
	}

	return renderer_;
}

void ShaderResource::MakeRenderer(void)
{
	if (IsUsedVS())
	{
		renderer_ = make_shared<Renderer>(psMaterial_, vsMaterial_);
		return;
	}

	renderer_ = make_shared<Renderer>(psMaterial_);
}
