#ifndef _PHAGE_SHADER_H
#define _PHAGE_SHADER_H

#include <stdlib.h>
#include <string>
#include <vector>

namespace Phage
{
	class PhageShader
	{
	public:
		static const int UNIFORM_FLOAT = 0;
		static const int UNIFORM_MATRIX = 1;
	public:
		PhageShader();
		~PhageShader();

		int Init();

		int LoadShader(std::string vertexFileName, std::string fragmentFileName);
		int UseShader(int index);

		int LoadUniformLocation(int index, std::string variable);
		int SetUniformValue(int index, int location, const float* matrix);
		int SetUniformValue(int index, int location, int i);
		int SetUniformValue(int index, int location, int numInts, int* i);
		int SetUniformValue(int index, int location, int numFloats, float* f, int uniformType = Phage::PhageShader::UNIFORM_FLOAT);
	private:
		std::vector<unsigned int> _vertexShaderVector;
		std::vector<unsigned int> _fragmentShaderVector;
		std::vector<unsigned int> _programVector;
	};
}

#endif
