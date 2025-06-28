import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyCircle } from "../2d/MyCircle.js";
import { MyCylinder } from "../cuboid/MyCylinder.js";
import { MyBlade } from "./MyBlade.js";

export class MyBucket extends CGFobject {
  constructor(scene) {
    super(scene);
    this.circle = new MyCircle(scene, 1, 12);
    this.cyl = new MyCylinder(scene, 12,1);
    this.holder = new MyBlade(scene,0.7, 0.05);
    this.water = new MyCircle(scene, 1, 12);
    this.offset = 0;
    this.filled = false;
    this.open = false;
    this.waterHeight = 0;
    this.waterMaxHeight = 0.55;
    this.waterMaterial = new CGFappearance(scene);
    const [r, g, b] = [25, 40, 255].map(x => x / 255);
    this.waterMaterial.setAmbient(r, g, b, 1);
    this.waterMaterial.setDiffuse(r, g, b, 1);
    this.waterMaterial.setSpecular(r / 10, g / 10, b / 10, 1);
    this.waterMaterial.setEmission(r, g, b, 1);
    this.waterMaterial.loadTexture("images/tree.jpg");
    this.waterMaterial.setTextureWrap("REPEAT", "REPEAT");
    this.waterMaterial.setShininess(10);
    
  }

  fillBucket() {
    this.filled = true;
    this.waterHeight = this.waterMaxHeight;
  }

  setOffset(offset) {
    offset = Math.max(0, offset - 0.3);
    this.offset = -offset;
  }

  display() {
    this.scene.pushMatrix();
      this.scene.pushMatrix();
        this.scene.translate(0,0,this.offset);
        this.scene.rotate(Math.PI / 2 * this.open, 0, 1, 0);
        this.scene.scale(0.4,0.4,0.7);
        this.circle.display();
      this.scene.popMatrix();
      // ------ //
      this.scene.pushMatrix();
        this.scene.translate(0,0,this.offset);
        this.scene.rotate(Math.PI / 2 * this.open, 0, 1, 0);
        this.scene.rotate(Math.PI, 1,0,0);
        this.scene.scale(0.4,0.4,0.7);
        this.circle.display();
      this.scene.popMatrix();
      // ------ //
      this.scene.pushMatrix();
        this.scene.translate(0,0,this.offset);
        this.scene.scale(0.4,0.4,0.7);
        this.cyl.display();
      this.scene.popMatrix();
      // ------ //
      this.scene.pushMatrix();
        this.scene.translate(0,0,this.offset);
        this.scene.translate(0,0,0.6);
        this.holder.display();
      this.scene.popMatrix();
      
      if (this.filled) {
        this.scene.pushMatrix();
          this.waterMaterial.apply();
          this.scene.translate(0,0,this.offset + this.waterHeight);
          this.scene.scale(0.4,0.4,0.7);
          this.circle.display();
        this.scene.popMatrix();
      }
    this.scene.popMatrix();
  }
  
}
