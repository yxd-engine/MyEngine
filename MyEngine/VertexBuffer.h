#pragma once


class VertexBuffer
{

private:
	unsigned int m_RendererID;
public:
	//size ����ָ�ж��ٸ�byte��
	VertexBuffer(const void* data, unsigned int size);
	~VertexBuffer();

	void Bind() const;
	void Unbind() const;

};