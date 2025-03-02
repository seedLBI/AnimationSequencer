#include "OpenGL.Quad.SymbolMSDF.Instance.h"

void Quad_SymbolMSDF_instance::AddSymbol(SymbolMSDF_Data* symbol) {
	symbols.push_back(symbol);
}

void Quad_SymbolMSDF_instance::DeleteSymbol(SymbolMSDF_Data* symbol) {
	for (int i = 0; i < symbols.size(); i++) {
		if (symbols[i] == symbol) {
			symbols.erase(symbols.begin() + i);
			break;
		}
	}
}

void Quad_SymbolMSDF_instance::Clear() {
	symbols.clear();
}

std::vector<SymbolMSDF_Data*>* Quad_SymbolMSDF_instance::GetSymbols() {
	return &symbols;
}



void Quad_SymbolMSDF_instance::Update(const float& time) {
	static int LastCount_symbols = 0;


	std::vector<InstanceSymbolData> data_instance;

	for (int i = 0; i < symbols.size(); i++) {
		data_instance.push_back(symbols[i]->GetData(time));
	}

	if (LastCount_symbols == symbols.size()) {
		// Update

		glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
		glBufferSubData(GL_ARRAY_BUFFER, 0, symbols.size() * sizeof(InstanceSymbolData), data_instance.data());
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	else {
		glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
		glBufferData(GL_ARRAY_BUFFER, symbols.size() * sizeof(InstanceSymbolData), data_instance.data(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	

}

void Quad_SymbolMSDF_instance::Draw() {
	glBindVertexArray(VAO);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, symbols.size());
}

void Quad_SymbolMSDF_instance::Init() {

	glGenBuffers(1, &VBO_Instanced);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, sizeof(InstanceSymbolData), nullptr, GL_DYNAMIC_DRAW);


	glEnableVertexAttribArray(2); // AtlasUV (vec4)[4*float]
	glVertexAttribDivisor(2, 1);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)0);

	glEnableVertexAttribArray(3); // Thikness (float)
	glVertexAttribDivisor(3, 1);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)(4 * sizeof(float)));

	glEnableVertexAttribArray(4); // ColorText (vec3)[3*float]
	glVertexAttribDivisor(4, 1);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)(5 * sizeof(float)));

	glEnableVertexAttribArray(5); // ColorOutline (vec3)[3*float]
	glVertexAttribDivisor(5, 1);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)(8 * sizeof(float)));

	glEnableVertexAttribArray(6); // MatrixTransform (mat4)[16*float] [4*float]
	glVertexAttribDivisor(6, 1);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)(11 * sizeof(float)));

	glEnableVertexAttribArray(7); // MatrixTransform (mat4)[16*float] [8*float]
	glVertexAttribDivisor(7, 1);
	glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 1) * sizeof(float)));

	glEnableVertexAttribArray(8); // MatrixTransform (mat4)[16*float] [12*float]
	glVertexAttribDivisor(8, 1);
	glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 2) * sizeof(float)));

	glEnableVertexAttribArray(9); // MatrixTransform (mat4)[16*float] [16*float]
	glVertexAttribDivisor(9, 1);
	glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 3) * sizeof(float)));


	glEnableVertexAttribArray(10); // Prev_MatrixTransform (mat4)[16*float] [4*float]
	glVertexAttribDivisor(10, 1);
	glVertexAttribPointer(10, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 4) * sizeof(float)));

	glEnableVertexAttribArray(11); // Prev_MatrixTransform (mat4)[16*float] [8*float]
	glVertexAttribDivisor(11, 1);
	glVertexAttribPointer(11, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 5) * sizeof(float)));

	glEnableVertexAttribArray(12); // Prev_MatrixTransform (mat4)[16*float] [12*float]
	glVertexAttribDivisor(12, 1);
	glVertexAttribPointer(12, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 6) * sizeof(float)));

	glEnableVertexAttribArray(13); // Prev_MatrixTransform (mat4)[16*float] [16*float]
	glVertexAttribDivisor(13, 1);
	glVertexAttribPointer(13, 4, GL_FLOAT, GL_FALSE, sizeof(InstanceSymbolData), (void*)((11 + 4 * 7) * sizeof(float)));


	glBindBuffer(GL_ARRAY_BUFFER, 0);

}


Quad_SymbolMSDF_instance::Quad_SymbolMSDF_instance(Font* font) : Quad() {
	this->font = font;
	Init();
}

Quad_SymbolMSDF_instance::~Quad_SymbolMSDF_instance() {

}
