import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MySealedCylinder } from "../cuboid/MySealedCylinder.js";

export class MyLandingGear extends CGFobject {
  constructor(scene) {
    super(scene);
    this.cyl = new MySealedCylinder(scene, 6);
    this.redMaterial = new CGFappearance(this.scene);
    let [r,g,b] = [220, 20, 20].map(x => x / 255);
    this.redMaterial.setAmbient(r,g,b, 0.5);
    this.redMaterial.setDiffuse(r,g,b, 0.3);
    this.redMaterial.setSpecular(r,g,b, 0.9);
    this.redMaterial.setShininess(20.);

    this.whiteMaterial = new CGFappearance(this.scene);
    [r,g,b] = [220, 220, 220].map(x => x / 255);
    this.whiteMaterial.setAmbient(r,g,b, 0.5);
    this.whiteMaterial.setDiffuse(r,g,b, 0.7);
    this.whiteMaterial.setSpecular(r,g,b, 0.8);
    this.whiteMaterial.setShininess(20.);
  }

  display() {
    this.scene.pushMatrix();
      this.scene.pushMatrix();
        this.redMaterial.apply();
        this.scene.translate(0,0.4,-1.8);
        this.scene.scale(0.05,0.05,1.8);
        this.cyl.display();
      this.scene.popMatrix();
      // ------ //
      this.scene.pushMatrix();
      this.redMaterial.apply();
        this.scene.translate(0,-0.4,-1.8);
        this.scene.scale(0.05,0.05,1.8);        
        this.cyl.display();
      this.scene.popMatrix();
      // ------ //
      this.scene.pushMatrix();
        this.whiteMaterial.apply();
        this.scene.translate(0,0.8,-1.8);
        this.scene.rotate(Math.PI / 2, 1, 0, 0);
        this.scene.scale(0.1,0.1,1.5);
        this.cyl.display();
      this.scene.popMatrix();
    this.scene.popMatrix();
  }
}
