import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyCylinder } from "../cuboid/MyCylinder.js";
import { MyUnitCube } from "../cuboid/MyUnitCube.js";
import { MyLandingGear } from "./MyLandingGear.js";
import { MyRotor } from "./MyRotor.js";

export class MyBody extends CGFobject {
  constructor(scene, diameter) {
    super(scene);
    this.diameter = diameter;
    this.main_body = new MyUnitCube(scene);
    this.rotor = new MyRotor(scene, 0.7,diameter, 0.3);
    this.exhaust = new MyCylinder(scene, 12, 1);
    this.rotating = false;
    this.yellowMaterial = new CGFappearance(this.scene);
    let [r,g,b] = [220, 220, 20].map(x => x / 255);
    this.yellowMaterial.setAmbient(r,g,b, 0.5);
    this.yellowMaterial.setDiffuse(r,g,b, 0.7);
    this.yellowMaterial.setSpecular(r,g,b, 0.3);
    this.yellowMaterial.setShininess(20.);

    this.exhaustMaterial = new CGFappearance(this.scene);
    this.exhaustMaterial.setAmbient(110/255,110/255,110/255, 0.6);
    this.exhaustMaterial.setDiffuse(90/255,90/255,90/255, 0.2);
    this.exhaustMaterial.setSpecular(220/255,220/255,220/255, 1.);
  }

  update(t) {
    this.rotor.rotating = this.rotating;
    this.rotor.update(t);
  }

  display() {
    this.scene.pushMatrix();
      this.scene.pushMatrix();
        this.yellowMaterial.apply();
        this.scene.scale(0.3,2,0.3);
        this.main_body.display();
      this.scene.popMatrix();
      
      this.scene.pushMatrix();
        this.yellowMaterial.apply();
        this.scene.translate(0.35,0,0);
        this.scene.scale(0.4,1.5,0.4);
        this.main_body.display();
      this.scene.popMatrix();
      
      this.scene.pushMatrix();
        this.yellowMaterial.apply();
        this.scene.translate(-0.35,0,0);
        this.scene.scale(0.4,1.5,0.4);
        this.main_body.display();
      this.scene.popMatrix();

      this.scene.pushMatrix();
        this.exhaustMaterial.apply();
        this.scene.translate(0.35,-0.7,0);
        this.scene.scale(0.2,0.5,0.2);
        this.scene.rotate(Math.PI / 2, 1, 0, 0);
        this.exhaust.display();
      this.scene.popMatrix();
      
      this.scene.pushMatrix();
        this.exhaustMaterial.apply();
        this.scene.translate(-0.35,-0.7,0);
        this.scene.scale(0.2,0.5,0.2);
        this.scene.rotate(Math.PI / 2, 1, 0, 0);
        this.exhaust.display();
      this.scene.popMatrix();
      
      this.scene.pushMatrix();
        this.scene.translate(0,0,0.7);
        this.rotor.display();
      this.scene.popMatrix();
      

    this.scene.popMatrix();
  }

  
}
