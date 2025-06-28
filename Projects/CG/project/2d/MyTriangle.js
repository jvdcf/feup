import { CGFobject } from '../../lib/CGF.js';

export class MyTriangle extends CGFobject {
    constructor(scene, width, height) {
        super(scene);
        this.width = width;
        this.height = height;
        this.depth = 0.1;
        this.initBuffers();
    }

    initBuffers() {
        this.vertices = [
            this.width / 2, 0, 0,
            0, this.height, 0,
            -this.width / 2, 0, 0,

            this.width / 2, 0, 0,
            0, this.height, 0,
            -this.width / 2, 0, 0,
        ];

        this.indices = [
            0, 1, 2,
            2, 1, 0,
        ];

        this.normals = [
            0, 0, 1,
            0, 0, 1,
            0, 0, 1,

            0, 0, -1,
            0, 0, -1,
            0, 0, -1,
        ];
        
        this.texCoords = [
            1, 0,
            0.5, 1,
            0, 0,

            1, 0,
            0.5, 1,
            0, 0,
        ];

        this.primitiveType = this.scene.gl.TRIANGLES;
        this.initGLBuffers();
    }
}