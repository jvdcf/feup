import { CGFobject, CGFappearance } from "../../lib/CGF.js";

export class MyCircle extends CGFobject {
  constructor(scene, radius, triangles) {
    super(scene);
    this.triangles = triangles + 1;
    this.radius = radius;
    this.initBuffers();
  }
  
  initBuffers() {
    this.vertices = [];
    this.normals = [];
    this.indices = [];
    this.texCoords = [];

    this.vertices.push(0, 0, 0);
    this.normals.push(0, 0, 1);
    this.texCoords.push(0.5, 0.5);

    const STEP = (Math.PI * 2.) / (this.triangles - 1);

    for (let i = 0; i < this.triangles; ++i) {
      const x = this.radius * Math.cos(i * STEP);
      const y = this.radius * Math.sin(i * STEP);
      const z = 0;
      this.vertices.push(x,y,z);  
      this.normals.push(0, 0, 1);
      this.texCoords.push((x / this.radius), (y / this.radius));
    }

    for (let i = 1; i <= this.triangles; ++i) {
      const i1 = i + 1 <= this.triangles ? i + 1 : (i + 1) % this.triangles;
      this.indices.push(0, i, i1);
    }

    this.primitiveType = this.scene.gl.TRIANGLES;
    this.initGLBuffers();
  }

  updateBuffers(radius, triangles) {
    this.radius = radius;
    this.triangles = triangles;
    // reinitialize buffers
    this.initBuffers();
    this.initNormalVizBuffers();
  }

}
