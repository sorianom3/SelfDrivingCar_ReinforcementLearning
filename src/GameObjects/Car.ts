import * as Phaser from 'phaser';
import Track from './Track';
import { Math } from 'phaser'
import { cos, sin, abs, rotate } from 'mathjs'
import { rayHit } from '../Utils';

export type CarData = {
    pos: Math.Vector2,
    vel: Math.Vector2,
    acc: Math.Vector2,
    maxSpeed: number,
    dragCoefficient: number
    rotation: number;
}

export default class Car extends Phaser.GameObjects.GameObject {

    public carInput: { drive: number, turn: number } = { drive: 0, turn: 0 };
    public car: CarData = {
        pos: new Math.Vector2(0, 0),
        vel: new Math.Vector2(0, 0),
        acc: new Math.Vector2(0, 0),
        maxSpeed: 4.5,
        dragCoefficient: 0.98,
        rotation: 90
    }
    private trackData: Track;
    private turnSpeed = 140;
    private carSprite: Phaser.GameObjects.Sprite;
    private line = new Phaser.Geom.Line();
    private wallLine: Phaser.GameObjects.Graphics;
    constructor(scene: Phaser.Scene, track: Track) {
        super(scene, 'Car');
        this.trackData = track;
        //create texture
        const textureKey = 'white-rect';
        let graphic = this.scene.add.graphics()
            .fillStyle(0xFFFFFF, 1.0)
            .fillRect(0, 0, 35, 15);
        graphic.generateTexture(textureKey, 35, 15);
        graphic.destroy();

        //create sprite
        this.car.pos.x = this.trackData.checkpoints[0].x1 + 20;
        this.car.pos.y = this.trackData.checkpoints[0].y1 - 40;
        this.carSprite = scene.make.sprite({
            key: textureKey,
            x: this.car.pos.y,
            y: this.car.pos.y
        }, true);
        this.carSprite.setOrigin(0.7);
        this.wallLine = this.scene.make.graphics({}, true);
    }

    collectPlayerInput() {
        let keys = [
            this.scene.input.keyboard?.addKey("W"),
            this.scene.input.keyboard?.addKey("S"),
            this.scene.input.keyboard?.addKey("A"),
            this.scene.input.keyboard?.addKey("D"),
        ];
        this.carInput.drive = Number(keys[0]!.isDown) - Number(keys[1]!.isDown);
        this.carInput.turn = Number(keys[3]!.isDown) - Number(keys[2]!.isDown);
    }

    collectAIInput() {
        //TODO Set NN to insert Input here
    }

    update(_dt: number): void {
        const { drive, turn } = this.carInput;
        this.car.rotation += turn * (this.car.vel.length() * 1.25) * this.turnSpeed * _dt;
        this.car.rotation = this.car.rotation % 360;
        let theta = Math.DEG_TO_RAD * this.car.rotation;
        let dir = new Math.Vector2(cos(theta), sin(theta));
        dir.normalize();
        let thrust = (drive < 0) ? -0.25 : drive;
        this.car.acc = dir.scale(thrust * _dt * this.car.maxSpeed);

        this.car.vel.add(this.car.acc);
        this.car.pos.add(this.car.vel);

        this.car.vel.scale(this.car.dragCoefficient);

        this.car.vel.x = (abs(this.car.vel.x) <= 0.001) ? 0 : this.car.vel.x;
        this.car.vel.y = (abs(this.car.vel.y) <= 0.001) ? 0 : this.car.vel.y;

        this.carSprite.setPosition(this.car.pos.x, this.car.pos.y);
        this.carSprite.setAngle(this.car.rotation);

        let rect = this.carSprite.getBounds();
        let rectPoint = Phaser.Geom.Rectangle.PerimeterPoint(rect, this.car.rotation);

        console.log(this.car.vel.length());
        
        let hitPoint = rayHit(new Math.Vector2(rectPoint.x, rectPoint.y), dir, this.trackData.segments);
        this.wallLine.setPosition(this.car.pos.x,this.car.pos.x);
    }

    checkHitWall(): boolean {
        let isHit = false;
        this.trackData.segments.forEach(line => {
            isHit = isHit || Phaser.Geom.Intersects.LineToRectangle(line, this.carSprite.getBounds());
        });
        return isHit;
    }

    checkHitCheckpoint(): boolean {
        let isHit = false;
        this.trackData.checkpoints.forEach(line => {
            isHit = isHit || Phaser.Geom.Intersects.LineToRectangle(line, this.carSprite.getBounds());
        });
        return isHit;
    }
}
