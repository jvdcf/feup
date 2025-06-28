import { CGFobject } from "../../lib/CGF.js";


export class MyRightAnglePrism extends CGFobject {
  constructor(scene) {
    super(scene);

    this.initBuffers();
  }

  initBuffers() {
    const STEP = Math.PI / 2;
    const v = [
      Math.cos(0 * STEP), Math.sin(0 * STEP), 0,
      Math.cos(1 * STEP), Math.sin(1 * STEP), 0,
      Math.cos(2 * STEP), Math.sin(2 * STEP), 0,
      Math.cos(0 * STEP), Math.sin(0 * STEP), 1,
      Math.cos(1 * STEP), Math.sin(1 * STEP), 1,
      Math.cos(2 * STEP), Math.sin(2 * STEP), 1,
    ];
    this.indices = [
      // CAPS
      0, 2, 1,
      3, 4, 5,
      // FACE 1
      0, 1, 4,
      4, 3, 0,
      // FACE 2
      1, 2, 5,
      5, 4, 1,
      // FACE 3
      2, 0, 3,
      3, 5, 2
    ];
    this.vertices = [];

    
    for (let i = 0; i < 3; ++i) {
      this.vertices = this.vertices.concat(v);  
    }

    // NORMALS
    // Verticals
    this.normals = [];
    for (let i = 0; i < 6; ++i) {
      if (i < 3) {
        this.normals.push(0,0,-1);
      } else {
        this.normals.push(0,0,1);   
      }
    }
    for (let i = 0; i < 2; ++i) {
      this.normals.push(0,-1, 0);
      this.normals.push(Math.sqrt(2)/2, Math.sqrt(2)/2, 0);
      this.normals.push(0,-1, 0);
    }
    
    for (let i = 0; i < 2; ++i) {
      this.normals.push(Math.sqrt(2)/2, Math.sqrt(2)/2, 0);
      this.normals.push(-Math.sqrt(2)/2, Math.sqrt(2)/2, 0);
      this.normals.push(-Math.sqrt(2)/2, Math.sqrt(2)/2, 0);
    }
    
    this.primitiveType = this.scene.gl.TRIANGLES;
    this.initGLBuffers();
  }
}
