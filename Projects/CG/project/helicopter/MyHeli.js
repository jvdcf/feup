import { CGFobject, CGFappearance } from "../../lib/CGF.js";
import { MyBody } from "./MyBody.js";

import { MyCockPit } from "./MyCockpit.js";
import { MyTail } from "./MyTail.js";
import { MyUnitCube } from "../cuboid/MyUnitCube.js";

const STATE = Object.freeze({
  STOPPED: 0,
  STARTING: 1,
  FLYING: 2,
  LANDING_ROTATE: 3,
  LANDING_MOVE: 4,
  LANDING_DOWN: 5,
  LOWERING_BUCKET: 6,
  FILL_BUCKET: 7,
  POURING_WATER: 8,
});

// https://easings.net/#easeInOutSine
function easeInOutSine(x) {
  return -(Math.cos(Math.PI * x) - 1) / 2;
}

import { MyLandingGear } from "./MyLandingGear.js";
import { MyBucket } from "./MyBucket.js";
import { MyRope } from "./MyRope.js";


function lerp(a,b,p) {
  return a * (1 - p) + b * p;
}

function invlerp(a,b,v) {
  return (v - a) / (b - a);
}

function mod2PI(angle) {
  return ((angle % (2 * Math.PI)) + (2 * Math.PI)) % (2 * Math.PI);
}

function clamp(min, max, v) {
  if (v < min) {
    return min;
  } else if (v > max) {
    return max;
  } else {
    return v;
  }
}


const EXTENDED_MAXIMUM = 5;
const BUCKET_ANIM_DUR = 4000;
const ACCEL_STEP = 0.001;
const AR_COEFF = 0.001  / 2;
const ACCEL_DECAY = 0.0001 * 5;

export class MyHeli extends CGFobject {
  constructor(scene, start_x, start_y, start_z) {
    super(scene);
    this.cockpit = new MyCockPit(scene);
    this.body = new MyBody(scene, 5);
    this.tail = new MyTail(scene, 1);
    this.landing_gear = new MyLandingGear(scene);
    this.bucket = new MyBucket(scene);
    this.rope = new MyRope(scene);
    this.start_x = start_x;
    this.start_y = start_y;
    this.start_z = start_z;
    this.animInstant = Date.now();
    
    this.begin = 0;
    this.end = 1;
    this.bpos = 0;
    this.extended = false;

    this.state = STATE.STOPPED;
    this.position = { x: start_x, y: start_y, z: start_z };
    this.orientation = - 2 * Math.PI / 3; // Angle around Y axis in radians
    this.speed = 0;
    this.pspeed = 0;
    this.tiltAngle = 0; // Tilt angle for the helicopter

    this.cruisingZ = 150; // Crusing altitude
    this.anglePerMs = 0.001; // Angle per millisecond for turning
    this.acceleration = 0; // Accelaration scalar.
    this.airResistance = 0.001;

    this.speedFactor = 1; // Speed multiplier for acceleration (used in MyInterface.js)

    this.animStartTime = 0;
    this.animEndTime = 1;
    this.animStartCoord = { x: null, y: null, z: null };
    this.animEndCoord = { x: null, y: null, z: null };   
  }

  display() {
    this.scene.pushMatrix();
    this.scene.translate(this.position.x, this.position.y, this.position.z + 5);
    this.scene.scale(10, 10, 10);
    this.scene.rotate(this.orientation - Math.PI / 2, 0, 0, 1);
    this.scene.rotate(-this.tiltAngle, 1, 0, 0);
    
    this.scene.translate(0,-1.8,0);
    this.scene.pushMatrix();
      this.scene.translate(0,3.3,0.2);
      this.cockpit.display();
    this.scene.popMatrix();
    
    this.scene.pushMatrix();
      this.scene.translate(0,1.8,0.5);
      this.body.display();
    this.scene.popMatrix();

    this.scene.pushMatrix();
      this.scene.translate(0,-0.6,0.4);
      this.scene.rotate(Math.PI / 2, 0, 0, 1);
      this.tail.display();
    this.scene.popMatrix();

    this.scene.pushMatrix();
      this.scene.translate(0,1.8,0.6);
      this.scene.rotate(Math.PI / 6, 0, 1, 0);
      this.landing_gear.display();
    this.scene.popMatrix();
    
    this.scene.pushMatrix();
      this.scene.translate(0,1.8,0.6);
      this.scene.rotate(-Math.PI / 6, 0, 1, 0);
      this.landing_gear.display();
    this.scene.popMatrix();

    this.scene.pushMatrix();
      this.scene.translate(0,1.8,0.2);
      this.rope.display();
    this.scene.popMatrix();

    this.scene.pushMatrix();
      this.scene.translate(0,1.8,-0.5);
      this.bucket.display();
    this.scene.popMatrix();
    
    this.scene.popMatrix();
  }

  update(t) {
    this.cruisingZ = this.start_z + 100;
    const delta_t = t - this.animInstant;
    this.animInstant = t;

    this.body.rotating = this.state !== STATE.STOPPED;
    this.tail.rotating = this.state !== STATE.STOPPED;
    this.calculateTilt(delta_t);
    this.decayAcceleration(delta_t);

    this.body.update(t);
    this.tail.update(t);
    if ((this.extended && this.bpos !== EXTENDED_MAXIMUM) || (!this.extended && this.bpos !== 0)) {
      const p_ = invlerp(this.begin, this.end, t);
      const p = p_ >= 1 ? 1 : p_; 
      this.bpos = lerp(this.bpos, this.extended ? EXTENDED_MAXIMUM : 0, p);
      this.bucket.setOffset(this.bpos);
      this.rope.update(this.bpos);
    }

    switch (this.state) {
      case STATE.STOPPED:
        this.toggle_bucket(false, t);
        return;

      case STATE.STARTING:
        // this.toggle_bucket(false, t);
        if (Math.round(this.position.z) < this.cruisingZ) {
          this.nextFrameAnim();
        } else {
          this.state = STATE.FLYING;
          this.speed = 0;
          this.position.z = this.cruisingZ;
        }
        break;

      case STATE.LANDING_ROTATE:
        this.speed = 0;
        this.pspeed = 0;
        this.toggle_bucket(true, t);
        const baseDirection = this.getBaseDirection(this.position.x, this.position.y);
        const orientationDelta = delta_t * this.anglePerMs * this.speedFactor;
        this.orientation = mod2PI(this.orientation);
        const orientationDiff = mod2PI(baseDirection - this.orientation);

        if (orientationDiff <= orientationDelta || orientationDiff >= (2 * Math.PI - orientationDelta)) {
          this.state = STATE.LANDING_MOVE;
          this.animStartTime = Date.now();
          const distance = Math.sqrt(
            Math.pow(this.position.x - this.start_x, 2) +
            Math.pow(this.position.y - this.start_y, 2)
          );
          this.animEndTime = this.animStartTime + distance * 20 / this.speedFactor;
          this.animStartCoord = { x: this.position.x, y: this.position.y, z: this.position.z };
          this.animEndCoord = { x: this.start_x, y: this.start_y, z: this.cruisingZ };

        } else {
          if (orientationDiff < Math.PI) {
            if (this.orientation + orientationDelta >= baseDirection) {
              this.orientation = baseDirection;
            } else {
              this.orientation += orientationDelta;
            }
          } else {
            if (this.orientation - orientationDelta <= baseDirection) {
              this.orientation = baseDirection;
            } else {
              this.orientation -= orientationDelta;
            }
          }
          this.orientation = mod2PI(this.orientation);
        }
        break;

      case STATE.LANDING_MOVE:
        this.toggle_bucket(true, t);
        if (Math.round(this.position.x) != this.start_x || Math.round(this.position.y) != this.start_y) {
          this.nextFrameAnim();
          const elapsedTime = this.animInstant - this.animStartTime;
          const animDuration = this.animEndTime - this.animStartTime;
          this.acceleration = Math.cos(this.orientation)
            * invlerp(this.animEndCoord.x,
              this.animStartCoord.x,
              this.animStartCoord.x + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.x - this.animStartCoord.x));
          this.acceleration += Math.sin(this.orientation)
            * invlerp(this.animEndCoord.y,
              this.animStartCoord.y,
              this.animStartCoord.y + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.y - this.animStartCoord.y));
          this.acceleration = clamp(0, invlerp(this.animStartCoord.y,
              this.animEndCoord.y,
              this.animStartCoord.y + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.y - this.animStartCoord.y)) / 2
              + invlerp(this.animStartCoord.x,
              this.animEndCoord.x,
              this.animStartCoord.x + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.x - this.animStartCoord.x)), this.acceleration);
        } else {
          this.state = STATE.LANDING_DOWN;
          this.position.x = this.start_x;
          this.position.y = this.start_y;
          this.animStartTime = Date.now();
          this.animEndTime = this.animStartTime + 1000;
          this.animStartCoord = { x: this.position.x, y: this.position.y, z: this.position.z };
          this.animEndCoord = { x: this.start_x, y: this.start_y, z: this.start_z };
        }
        break;

      case STATE.LANDING_DOWN:
        this.toggle_bucket(false, t);
        if (Math.round(this.position.z) > this.start_z) {
          this.nextFrameAnim();
        } else {
          this.state = STATE.STOPPED;
          this.position.z = this.start_z;
          this.speed = 0;
          this.pspeed = 0;
        }
        break;

      case STATE.LOWERING_BUCKET:
        this.toggle_bucket(true, t);
        if (this.animEndTime > Date.now()) {
          this.nextFrameAnim();
        } else {
          this.state = STATE.FILL_BUCKET;
          this.position.z = 25;
          this.bucket.fillBucket();
          this.speed = 0;
          this.pspeed = 0;
        }
        break;

      case STATE.FLYING:
        this.toggle_bucket(true, t);
        this.calculateSpeed(delta_t);
        const vector = this.calculateVector();
        this.position.x += (vector.x * delta_t) / 1000;
        this.position.y += (vector.y * delta_t) / 1000;
        this.position.z += (vector.z * delta_t) / 1000;
        this.pspeed = this.speed;
        break;

      case STATE.POURING_WATER:
        if (this.animEndTime > Date.now()) {
          this.bucket.waterHeight = this.bucket.waterMaxHeight * (1 - easeInOutSine((t - this.animStartTime) / (this.animEndTime - this.animStartTime)));
          // TODO
        } else {
          this.state = STATE.FLYING;
          this.bucket.filled = false;
          this.bucket.open = false;
        }
    }
  }

  isLanded() {
    return this.state === STATE.STOPPED; 
  }

  isBeingAnimated() {
    return !(this.state === STATE.STOPPED ||
      this.state === STATE.FLYING ||
      this.state === STATE.FILL_BUCKET);
  }

  toggle_bucket(extended, t) {
    if (extended !== this.extended) {
      if (this.bpos === 0 || this.bpos == EXTENDED_MAXIMUM) {
        this.begin = t;
        // 1 second
        this.end = t + BUCKET_ANIM_DUR;
      } else {
        const dinvl = BUCKET_ANIM_DUR * invlerp(this.extended ? 0 : EXTENDED_MAXIMUM, this.extended ? EXTENDED_MAXIMUM : 0, this.bpos);
        this.begin = t;
        this.end = t + dinvl;
      }
    }
    this.extended = extended;
  }

  nextFrameAnim() {
    // Animation based on elapsed time since animation start
    const elapsedTime = this.animInstant - this.animStartTime;
    const animDuration = this.animEndTime - this.animStartTime;

    if (elapsedTime >= 0 && elapsedTime <= animDuration) {
      this.position.x = this.animStartCoord.x + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.x - this.animStartCoord.x);
      this.position.y = this.animStartCoord.y + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.y - this.animStartCoord.y);
      this.position.z = this.animStartCoord.z + easeInOutSine(elapsedTime / animDuration) * (this.animEndCoord.z - this.animStartCoord.z);
    } else {
      console.warn("Animation time exceeded duration.");
    }
  }

  turnLeft(t) {
    if (this.state !== STATE.FLYING) return;
    const delta_t = t - this.animInstant;
    this.orientation += delta_t * this.anglePerMs * this.speedFactor;
    this.orientation = this.orientation % (2 * Math.PI);
  }

  turnRight(t) {
    if (this.state !== STATE.FLYING) return;
    const delta_t = t - this.animInstant;
    this.orientation -= delta_t * this.anglePerMs * this.speedFactor;
    this.orientation = this.orientation % (2 * Math.PI);
  }

  accelerate(t) {
    if (this.state !== STATE.FLYING) return;
    const delta_t = t - this.animInstant;
    this.acceleration += delta_t * ACCEL_STEP * this.speedFactor;
    this.acceleration = clamp(-8/10, 8/10, this.acceleration);
    if (this.speed < 0) this.speed = 0;
  }

  brake(t) {
    if (this.state !== STATE.FLYING) return;
    const delta_t = t - this.animInstant;
    if (this.speed > 0) this.acceleration -= delta_t * ACCEL_STEP * this.speedFactor;
    this.acceleration = clamp(-8/10, 8/10, this.acceleration);
  }

  start_and_raise() {
    if (this.state !== STATE.STOPPED && this.state !== STATE.FILL_BUCKET) return;
    this.state = STATE.STARTING;
    this.animStartTime = Date.now();
    this.animEndTime = this.animStartTime + 2000;
    this.animStartCoord = { x: this.position.x, y: this.position.y, z: this.position.z };
    this.animEndCoord = { x: this.position.x, y: this.position.y, z: this.cruisingZ };
  }

  stop_and_lower() {
    if (this.state !== STATE.FLYING) return;
    if (this.bucket.filled) return;
    this.state = STATE.LANDING_ROTATE;
  }

  get_water(x, y) {
    if (this.state !== STATE.FLYING) return;
    this.state = STATE.LOWERING_BUCKET;
    this.animStartTime = Date.now();
    this.animEndTime = this.animStartTime + 3000;
    this.animStartCoord = { x: this.position.x, y: this.position.y, z: this.position.z };
    this.animEndCoord = { x: x, y: y, z: 25 };
  }

  drop_water() {
    if (this.state !== STATE.FLYING) return;
    if (!this.bucket.filled) return;
    this.state = STATE.POURING_WATER;
    this.animStartTime = Date.now();
    this.animEndTime = this.animStartTime + 5000;
    this.bucket.open = true;
  }

  reset() {
    this.position = { x: this.start_x, y: this.start_y, z: this.start_z };
    this.orientation = 0;
    this.speed = 0;
    this.state = STATE.STOPPED;
    this.bucket.filled = false;
  }

  calculateVector() {
    return {
      x: Math.cos(this.orientation) * this.speed * this.speedFactor,
      y: Math.sin(this.orientation) * this.speed * this.speedFactor,
      z: (this.cruisingZ - this.position.z) / 2,
    };
  }

  calculateTilt() {
    this.tiltAngle = Math.PI / 3 * this.acceleration;
  }

  calculateSpeed(delta_t) {
    this.pspeed = this.speed;
    this.speed += this.acceleration * delta_t * this.speedFactor;
    this.speed -= Math.pow(this.speed, 2) * AR_COEFF;
    this.speed = clamp(0, Number.MAX_SAFE_INTEGER, this.speed);
  }

  decayAcceleration(delta_t) {
    if (this.acceleration === 0) return; 
    const sign = Math.abs(this.acceleration) / this.acceleration;
    this.acceleration -= this.speedFactor * ((this.acceleration > 0) ? ACCEL_DECAY * delta_t : -ACCEL_DECAY * delta_t);
    if (sign !== Math.abs(this.acceleration) / this.acceleration) this.acceleration = 0;
  }

  getBaseDirection(x, y) {
    const vector = {
      x: this.start_x - x,
      y: this.start_y - y,
      z: 0,
    };
    return mod2PI(Math.atan2(vector.y, vector.x));
  }
}
