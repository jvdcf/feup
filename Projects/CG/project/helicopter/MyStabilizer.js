import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyRightAnglePrism } from "./MyRightAnglePrism.js";

export class MyStabilizer extends CGFobject {
  constructor(scene,) {
    super(scene);
    this.control_surface = new MyRightAnglePrism(scene); // Triangle;
    
    this.redMaterial = new CGFappearance(this.scene);
    this.yellowMaterial = new CGFappearance(this.scene);
    let [r,g,b] = [220, 220, 20].map(x => x / 255);
    this.yellowMaterial.setAmbient(r,g,b, 0.5);
    this.yellowMaterial.setDiffuse(r,g,b, 0.7);
    this.yellowMaterial.setSpecular(r,g,b, 0.3);
    this.yellowMaterial.setShininess(20.);
  }

  display() {
    this.scene.pushMatrix();
      this.scene.pushMatrix();
        this.scene.translate(0,-0.05,Math.sqrt(2)/4);
        this.scene.rotate(Math.PI, 0, 0, 1);
        this.scene.rotate(-Math.PI / 4, 0, 1, 0);
        this.scene.rotate(Math.PI, 0, 1, 0);
        this.scene.rotate(Math.PI / 2, 1, 0, 0);
        this.scene.scale(Math.sqrt(2)/2, Math.sqrt(2)/2, 0.1);
        this.yellowMaterial.apply();
        this.control_surface.display();
      this.scene.popMatrix();
      
    this.scene.popMatrix();
  }
}
