#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererID;
	unsigned int m_Count; // quanti indici ha 
public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	~IndexBuffer();

	// const perche non modificano nulla
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }
};