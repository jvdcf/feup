import { CGFobject } from "../../lib/CGF.js";
/**
 * MyUnitCube
 * @constructor
 * @param scene - Reference to MyScene object
 */
export class MyUnitCube extends CGFobject {
  constructor(scene) {
    super(scene);
    this.initBuffers();
  }

  initBuffers() {
		this.vertices = [		// Idx x      y      z
			-0.5, -0.5, -0.5,	// 0 - Left , Bottom, Back
			 0.5, -0.5, -0.5,	// 1 - Right, Bottom, Back
			-0.5,  0.5, -0.5,	// 2 - Left , Top   , Back
			 0.5,  0.5, -0.5,	// 3 - Right, Top   , Back
			-0.5, -0.5,  0.5,	// 4 - Left , Bottom, Front
			 0.5, -0.5,  0.5,	// 5 - Right, Bottom, Front
			-0.5,  0.5,  0.5,	// 6 - Left , Top   , Front
			 0.5,  0.5,  0.5,	// 7 - Right, Top   , Front

			-0.5, -0.5, -0.5,	// The same vertices repeated twice
			 0.5, -0.5, -0.5,
			-0.5,  0.5, -0.5,
			 0.5,  0.5, -0.5,
			-0.5, -0.5,  0.5,
			 0.5, -0.5,  0.5,
			-0.5,  0.5,  0.5,
			 0.5,  0.5,  0.5,
			-0.5, -0.5, -0.5,	
			 0.5, -0.5, -0.5,	
			-0.5,  0.5, -0.5,
			 0.5,  0.5, -0.5,
			-0.5, -0.5,  0.5,
			 0.5, -0.5,  0.5,
			-0.5,  0.5,  0.5,
			 0.5,  0.5,  0.5
		];

		//Counter-clockwise reference of vertices
		this.indices = [];
		let indices = [
			2, 1, 0,	// Back face
			2, 3, 1,	// ''
			3, 5, 1,	// Right face
			3, 7, 5,	// ''
			7, 4, 5,	// Front face
			7, 6, 4,	// ''
			6, 0, 4,	// Left face
			6, 2, 0,	// ''
			6, 3, 2,	// Top face
			6, 7, 3,	// ''
			0, 5, 4,	// Bottom face
			0, 1, 5,	// ''
		];

		this.indices = indices;

		this.normals = [
			0, 0, -1,
			0, 0, -1,
			0, 0, -1,
			0, 0, -1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			0, 0, 1,
			-1, 0, 0,
			1, 0, 0,
			-1, 0, 0,
			1, 0, 0,
			-1, 0, 0,
			1, 0, 0,
			-1, 0, 0,
			1, 0, 0,
			0, -1, 0,
			0, -1, 0,
			0, 1, 0,
			0, 1, 0,
			0, -1, 0,
			0, -1, 0,
			0, 1, 0,
			0, 1, 0,
		]

    //The defined indices (and corresponding vertices)
    //will be read in groups of three to draw triangles
    this.primitiveType = this.scene.gl.TRIANGLES;

    this.initGLBuffers();
  }
}
