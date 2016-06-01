#include "../lib/glew/glew.h"
#include "../include/renderer.h"
#include "../include/vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

Ptr<Renderer> Renderer::mInstance = nullptr;

Renderer::Renderer() {
	String vertexShader = String("data/vertex.glsl");
	String fragmentShader = String("data/fragment.glsl");
	mDefaultProgram = CreateProgram(vertexShader, fragmentShader);
	UseProgram(mDefaultProgram);
}

void Renderer::Setup3D() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::SetMatrices(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection) {
	glm::mat4 mvp = projection * view * model;
	glm::mat4 normalMat = glm::transpose(glm::inverse(view * model));
	glUniformMatrix4fv(mMVPLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glUniformMatrix4fv(mNormalMatLoc, 1, GL_FALSE, glm::value_ptr(normalMat));
	glUniformMatrix4fv(mModelViewMatLoc, 1, GL_FALSE, glm::value_ptr(view * model));
}

void Renderer::SetViewport(int x, int y, int w, int h) {
	glViewport(x, y, w, h);
	glScissor(x, y, w, h);
}

void Renderer::ClearColorBuffer(const glm::vec3& color) {
	glClearColor(color.r, color.g, color.b, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::ClearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}

uint32 Renderer::LoadTexture(const String& filename, uint32& width, uint32& height) {
	// Load bitmap
	int w, h, c;
	uint8* buffer = stbi_load(filename.ToCString(), &w, &h, &c, 4);
	if (!buffer) return 0;
	width = w;
	height = h;

	// Flip lines
	uint8* newBuffer = (uint8*)malloc(width*height * 4);
	int line = 0;
	for (int y = height - 1; y >= 0; y--) {
		memcpy(&newBuffer[line*width * 4], &buffer[y*width * 4], width * 4);
		line++;
	}
	free(buffer);
	buffer = newBuffer;

	// Create GL texture
	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Unload bitmap
	free(buffer);

	return handle;
}

void Renderer::FreeTexture(uint32 tex) {
	glDeleteTextures(1, &tex);
}

void Renderer::SetTexture(uint32 tex) {
	glBindTexture(GL_TEXTURE_2D, tex);
	if (!tex)
		glUniform1i(mUsesTextureLoc, 0);
	else
		glUniform1i(mUsesTextureLoc, 1);
}

void Renderer::SetDiffuse(const glm::vec3& color) {
	glUniform3f(mDiffuseLoc, color.r, color.g, color.b);
}

void Renderer::SetAmbient(const glm::vec3& color) {
	glUniform3f(mAmbientLoc, color.r, color.g, color.b);
}

void Renderer::SetShininess(uint8 shininess) {
	glUniform1i(mShininessLoc, shininess);
}

void Renderer::EnableLight(uint32 index, bool enabled) {
	mLightsEnabled[index] = true;
}

void Renderer::SetLightData(uint32 index, const glm::vec4& vector,
	const glm::vec3& color, float attenuation) {
	mLightsPos[index] = vector;
	mLightsColor[index] = color;
	mLightsAtt[index] = attenuation;

	glUniform4f(mLightsPosLoc[index], mLightsPos[index].x,
		mLightsPos[index].y, mLightsPos[index].z, 1);
	glUniform3f(mLightsColorLoc[index], mLightsColor[index].x,
		mLightsColor[index].y, mLightsColor[index].z);
	glUniform1f(mLightsAttLoc[index], mLightsAtt[index]);
}

uint32 Renderer::CreateBuffer() {
	uint32 buffer;
	glGenBuffers(1, &buffer);
	return buffer;
}

void Renderer::FreeBuffer(uint32 buffer) {
	glDeleteBuffers(1, &buffer);
}

void Renderer::SetVertexBufferData(uint32 vertexBuffer, const void* data, uint32 dataSize) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, dataSize, data, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Renderer::SetIndexBufferData(uint32 indexBuffer, const void* data, uint32 dataSize) {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataSize, data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Renderer::DrawBuffers(uint32 vertexBuffer, uint32 indexBuffer, uint32 numIndices) {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glEnableVertexAttribArray(mVPosLoc);
	glVertexAttribPointer(mVPosLoc, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (const void*)offsetof(Vertex, mPosition));
	glEnableVertexAttribArray(mVTexLoc);
	glVertexAttribPointer(mVTexLoc, 2, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (const void*)offsetof(Vertex, mTexCoords));
	glEnableVertexAttribArray(mVNormLoc);
	glVertexAttribPointer(mVNormLoc, 3, GL_FLOAT, GL_FALSE,
		sizeof(Vertex), (const void*)offsetof(Vertex, mNormal));

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32 Renderer::CreateProgram(const String& vertex, const String& fragment) {
	GLuint vertSh = glCreateShader(GL_VERTEX_SHADER);
	String str = String::Read(vertex);
	const char * vertCode = str.ToCString();
	glShaderSource(vertSh, 1, &vertCode, nullptr);

	GLuint fragSh = glCreateShader(GL_FRAGMENT_SHADER);
	str = String::Read(fragment);
	const char * fragCode = str.ToCString();
	glShaderSource(fragSh, 1, &fragCode, nullptr);

	//compiler Shaders and check if their compilation was ok
	GLint retCode;
	char errorLog[1024];
	//vertex
	glCompileShader(vertSh);
	glGetShaderiv(vertSh, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(vertSh, sizeof(errorLog), nullptr, errorLog);
		printf("Vertex shader error:\n");
		printf(errorLog);
		mProgramError = errorLog;
		glDeleteShader(vertSh);
		return 0;
	}
	//fragment
	glCompileShader(fragSh);
	glGetShaderiv(fragSh, GL_COMPILE_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetShaderInfoLog(fragSh, sizeof(errorLog), nullptr, errorLog);
		printf("Fragment shader error:\n");
		printf(errorLog);
		mProgramError = errorLog;
		glDeleteShader(fragSh);
		return 0;
	}

	//create program
	GLuint program = glCreateProgram();
	glAttachShader(program, vertSh);
	glAttachShader(program, fragSh);
	//link program
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &retCode);
	if (retCode == GL_FALSE) {
		glGetProgramInfoLog(program, sizeof(errorLog), nullptr, errorLog);
		printf("Program linking error:\n");
		printf(errorLog);
		mProgramError = errorLog;
		glDeleteShader(vertSh);
		glDeleteShader(fragSh);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

void Renderer::FreeProgram(uint32 program) {
	glDeleteProgram(program);
}

void Renderer::UseProgram(uint32 program) {
	if (!program) {
		glUseProgram(mDefaultProgram);
	} else {
		glUseProgram(program);
		mMVPLoc = glGetUniformLocation(program, "MVP");
		mNormalMatLoc = glGetUniformLocation(program, "NormalMatrix");
		mModelViewMatLoc = glGetUniformLocation(program, "ModelView");
		mTexSamplerLoc = glGetUniformLocation(program, "texSampler");
		glUniform1i(mTexSamplerLoc, 0);
		mDiffuseLoc = glGetUniformLocation(program, "diffuse");
		mAmbientLoc = glGetUniformLocation(program, "ambient");
		mShininessLoc = glGetUniformLocation(program, "shininess");
		mUsesTextureLoc = glGetUniformLocation(program, "usesTexture");
		mVPosLoc = glGetAttribLocation(program, "vpos");
		mVTexLoc = glGetAttribLocation(program, "vuv");
		mVNormLoc = glGetAttribLocation(program, "vnormal");

		/*for (uint16 i = 0; i < MAX_LIGHTS; ++i) {
			String str = String("lightEnabled[") + String::FromInt(i) + String("]");
			mLightsEnabledLoc[i] = glGetUniformLocation(program, str.ToCString());
		}*/
		for (uint16 i = 0; i < MAX_LIGHTS; ++i) {
			String str = String("lightPos[") + String::FromInt(i) + String("]");
			mLightsPosLoc[i] = glGetUniformLocation(program, str.ToCString());
		}
		for (uint16 i = 0; i < MAX_LIGHTS; ++i) {
			String str = String("lightColor[") + String::FromInt(i) + String("]");
			mLightsColorLoc[i] = glGetUniformLocation(program, str.ToCString());
		}
		for (uint16 i = 0; i < MAX_LIGHTS; ++i) {
			String str = String("lightAtt[") + String::FromInt(i) + String("]");
			mLightsAttLoc[i] = glGetUniformLocation(program, str.ToCString());
		}
	}
}

const String& Renderer::GetProgramError() {
	return mProgramError;
}