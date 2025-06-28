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
			 0.5,  0.5,  0.5	// 7 - Right, Top   , Front
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
			5, 1, 0		// ''
		];

		//The defined indices (and corresponding vertices)
		//will be read in groups of three to draw triangles
		this.primitiveType = this.scene.gl.TRIANGLES;

		this.initGLBuffers();
	}

}

