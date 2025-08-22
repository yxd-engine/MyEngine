#pragma once


class VertexBuffer
{

private:
	unsigned int m_RendererID;
public:
	//size 变量指有多少个byte数
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

};