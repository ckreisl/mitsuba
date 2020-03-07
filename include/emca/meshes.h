/*
 * vsdmeshes.h
 *
 *  Created on: Aug 9, 2018
 *      Author: ckreisl
 */

#ifndef INCLUDE_EMCA_VSDMESHES_H_
#define INCLUDE_EMCA_VSDMESHES_H_

#include <emca/datatypes.h>
#include <emca/platform.h>
#include <emca/stream.h>
#include <emca/mesh.h>

EMCA_NAMESPACE_BEGIN

class MTS_EXPORT_EMCA Meshes {
public:

	Meshes();
	~Meshes();

	void addMesh(Mesh &mesh) { m_meshes.push_back(mesh); }
	void clear() { m_meshes.clear(); }

	void serialize(Stream *stream);

private:
	std::vector<Mesh> m_meshes;

};

EMCA_NAMESPACE_END

#endif /* INCLUDE_EMCA_VSDMESHES_H_ */
