import {CGFobject} from '../lib/CGF.js';
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
		this.indices = [
			0, 1, 2,	// Back face
			1, 3, 2,	// ''
			1, 5, 3,	// Right face
			5, 7, 3,	// ''
			5, 4, 7,	// Front face
			4, 6, 7,	// ''
			4, 0, 6,	// Left face
			0, 2, 6,	// ''
			2, 3, 6,	// Top face
			3, 7, 6,	// ''
			4, 5, 0,	// Bottom face
			5, 1, 0,	// ''

			8, 9, 10,	// The same indices repeated twice
			9, 11, 10,
			9, 13, 11,
			13, 15, 11,
			13, 12, 15,
			12, 14, 15,
			12, 8, 14,
			8, 10, 14,
			10, 11, 14,
			11, 15, 14,
			12, 13, 8,
			13, 9, 8,
			16, 17, 18,	
			17, 19, 18,
			17, 21, 19,
			21, 23, 19,
			21, 20, 23,
			20, 22, 23,
			20, 16, 22,
			16, 18, 22,
			18, 19, 22,
			19, 23, 22,
			20, 21, 16,
			21, 17, 16
		];

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

