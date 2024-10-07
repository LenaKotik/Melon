#include "Melon.hpp"

void Melon::DefaultTransform3D::SetTransform(Shader* sh, const CoordinateSystem3D& T)
{
	sh->SetMatrix4(T.TransformationFrom(), "model");
}
void Melon::DefaultTransform2D::SetTransform(Shader* sh, const CoordinateSystem2D& T)
{
	sh->SetMatrix4(T.TransformationFrom(), "model");
}
bool Melon::ColorGraphics::SetColor(Color c, int)
{
	Color_ = c;
	return 1;
}
void Melon::ColorGraphics::SetGraphics(Shader* sh)
{
	sh->SetColor(Color_, "Color");
}
void Melon::TextureGraphics::SetGraphics(Shader* sh)
{
	sh->SetTexture(Texture_, "Texture");
}
bool Melon::TextureGraphics::SetTexture(Texture* t, int id)
{
	Texture_ = t;
	return 1;
}
void Melon::BrushGraphics::SetGraphics(Shader* sh)
{
	sh->SetBrush(Brush_, "brush");
}
bool Melon::BrushGraphics::SetColor(Color c, int id)
{
	Brush_ = c;
	return 1;
}
bool Melon::BrushGraphics::SetTexture(Texture* t, int id)
{
	Brush_ = t;
	return 1;
}
bool Melon::BrushGraphics::SetBrush(Brush b, int id)
{
	Brush_ = b;
	return 1;
}
void Melon::MaterialGraphics::SetGraphics(Shader* sh)
{
	sh->SetMaterial(Material_, "material");
}
bool Melon::MaterialGraphics::SetColor(Color c, int id)
{
	switch (id)
	{
	case 0:
		Material_.Albedo = c;
		break;
	case 1:
		Material_.Diffuse = c;
		break;
	case 2:
		Material_.Specular = c;
		break;
	default:
		return 0;
	}
	return 1;
}
bool Melon::MaterialGraphics::SetTexture(Texture* t, int id)
{
	switch (id)
	{
	case 0:
		Material_.Albedo = t;
		break;
	case 1:
		Material_.Diffuse = t;
		break;
	case 2:
		Material_.Specular = t;
		break;
	default:
		return 0;
	}
	return 1;
}
bool Melon::MaterialGraphics::SetBrush(Brush b, int id)
{
	switch (id)
	{
	case 0:
		Material_.Albedo = b;
		break;
	case 1:
		Material_.Diffuse = b;
		break;
	case 2:
		Material_.Specular = b;
		break;
	default:
		return 0;
	}
	return 1;
}
bool Melon::MaterialGraphics::SetMaterial(Material m, int id)
{
	Material_ = m;
	return 1;
}
void Melon::BorderGraphics::SetGraphics(Shader* sh)
{
	sh->SetColor(Color_, "Color");
	sh->SetColor(BorderColor, "BorderColor");
	sh->SetFloat(BorderWidth, "BorderWidth");
}
bool Melon::BorderGraphics::SetColor(Color c, int id)
{
	if (id == 0)
	{
		Color_ = c;
		return true;
	}
	if (id == 1)
	{
		BorderColor = c;
		return true;
	}
	return false;
}
bool Melon::BorderGraphics::SetWidth(float w)
{
	BorderWidth = w;
	return true;
}
void Melon::CubeMapGraphics::SetGraphics(Shader* sh)
{
	sh->SetCubeMap(CubeMap_, "CubeMap");
}
bool Melon::CubeMapGraphics::SetCubeMap(CubeMap* t, int id)
{
	CubeMap_ = t;
	return true;
}

void Melon::RenderedObject2D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

void Melon::RenderedObject2D::Draw(RenderTarget* target)
{
	target->Bind();
	Shader_.Use();

	Camera2D* cam = (Camera2D*)target->MainCamera; // abstract it

	Matrix4 ortho = Matrix4::Ortho(target->GetAspect(), 0, 100);

	Transform->SetTransform(&Shader_, T);
	Shader_.SetMatrix4(cam->GetView(), "view"); // this 
	Shader_.SetMatrix4(ortho, "projection");
	
	Graphics->SetGraphics(&Shader_);
	
	Renderer_.Draw();
}


void Melon::RenderedObject3D::Delete()
{
	Shader_.Delete();
	Renderer_.Delete();
}

void Melon::RenderedObject3D::Draw(RenderTarget* target)
{
	target->Bind();
	Shader_.Use();
	Transform->SetTransform(&Shader_, T);
	Camera3D* cam = (Camera3D*)target->MainCamera; // this too
	Matrix4 persp = Matrix4::Perspective(cam->FOV, target->GetAspect(), 0.1f, 100.0f);
	Shader_.SetMatrix4(cam->GetView(), "view");
	Shader_.SetMatrix4(persp, "projection");
	Graphics->SetGraphics(&Shader_); 
	Renderer_.Draw();
}

Melon::Skybox* Melon::SkyboxFactory::Create(CubeMap m)
{
	Mesh mesh = Helpers::Meshes::Cube();
	Shader* s = Helpers::ShaderLib::LoadBasic("Skybox");
	if (!s) return 0;
	Skybox* box = new Skybox(s, &mesh);
	box->CubeMap_ = m;
	box->Transform = (ShaderTransform3D*)(new DefaultTransform3D);
	box->Graphics = (ShaderGraphics*)(new CubeMapGraphics);
	return box;
}

void Melon::Skybox::Draw(RenderTarget* target)
{
	target->Bind();
	glDepthMask(false);
	Shader_.Use();
	Transform->SetTransform(&Shader_, T);
	
	Camera3D* cam = (Camera3D*)target->MainCamera; // this too
	Matrix4 persp = Matrix4::Perspective(cam->FOV, target->GetAspect(), 0.1f, 100.0f);
	
	Matrix4 view = cam->GetView();

	for (int i = 0; i < 3; i++)
	{
		view.Value[3][i] = 0;
		view.Value[i][3] = 0;
	}
	view.Value[3][3] = 1;

	Shader_.SetMatrix4(view, "view");
	Shader_.SetMatrix4(persp, "projection");
	
	((CubeMapGraphics*)Graphics)->CubeMap_ = &CubeMap_;
	Graphics->SetGraphics(&Shader_);
	
	Renderer_.Draw();
	glDepthMask(true);
}

void Melon::DepthBuffer::Bind()
{
	glBindRenderbuffer(GL_RENDERBUFFER, handle);
}

Melon::DepthBuffer::DepthBuffer(Vector2 size) : size_(size)
{
	glGenRenderbuffers(1, &handle);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, (GLsizei)size.x, (GLsizei)size.y);
}

void Melon::DepthBuffer::Delete()
{
	glDeleteRenderbuffers(1, &handle);
}

Melon::FrameBuffer::FrameBuffer(Vector2 size) : size_(size), depthAttachment(0)
{
	glGenFramebuffers(1, &handle);
}

void Melon::FrameBuffer::ColorAttachment(Texture* tex)
{
	// TODO: add support for multiple color attachments
	Bind();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex->handle, 0);
	colorAttachments.PushBack(tex);
}

void Melon::FrameBuffer::DepthAttachment(DepthBuffer* buffer)
{
	Bind();
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer->handle);
	depthAttachment = buffer;
}

Melon::Texture* Melon::FrameBuffer::GetColorAttachment(int idx)
{
	return colorAttachments[idx];
}

void Melon::FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, handle);
}

void Melon::FrameBuffer::Delete()
{
	glDeleteFramebuffers(1, &handle);
	for (Texture* t : colorAttachments)
		t->Delete();
	depthAttachment->Delete();
}

Melon::FrameBuffer* Melon::FrameBufferFactory::GetBasic(Vector2 size)
{
	FrameBuffer* res = new FrameBuffer(size);
	res->Bind();
	TextureData td((Byte*)NULL, size.x, size.y, 4, GL_MIRRORED_REPEAT, GL_LINEAR);
	Texture* t = new Texture(td);
	res->ColorAttachment(t);
	if (Windowing::depth_)
	{
		DepthBuffer* db = new DepthBuffer(size);
		res->DepthAttachment(db);
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
		return res;
#ifdef DEBUG_OUTPUT
	std::cout << "FRAMEBUFFER ERROR: Framebuffer is incomplete!\nFramebuffer status: " << (glCheckFramebufferStatus(GL_FRAMEBUFFER)) << std::endl;
#endif // DEBUG_OUTPUT
	return 0;
}