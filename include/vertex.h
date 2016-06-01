#ifndef UGINE_VERTEX_H
#define UGINE_VERTEX_H

#include "types.h"

struct Vertex {
	Vertex(): mPosition(0, 0, 0), mTexCoords(0, 0), mNormal(0, 0, 0) {}
	Vertex(const glm::vec3& position, const glm::vec2& texCoords, const double normal):
		mPosition(position), mTexCoords(texCoords), mNormal(normal) {}
	bool operator==(const Vertex& other) const {
		return mPosition == other.mPosition
			&& mTexCoords == other.mTexCoords && mNormal == other.mNormal;
	}

	glm::vec3 mPosition;
	glm::vec2 mTexCoords;
	glm::vec3 mNormal;
};

#endif // UGINE_VERTEX_H
