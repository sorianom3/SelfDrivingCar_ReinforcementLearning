import * as Phaser from 'phaser';
import Track from './Track';
import { Math } from 'phaser'
import { cos, sin, abs, rotate, log } from 'mathjs'
import { rayHit, rayHits } from '../Utils';

export type CarData = {
    pos: Math.Vector2,
    vel: Math.Vector2,
    acc: Math.Vector2,
    maxSpeed: number,
    dragCoefficient: number
    rotation: number;
}

export type VisionData = {
    wallDist: number[],
    goalDist: number
    speed: number
    position: Math.Vector2
};

const GOAL = 8;
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
    public visionData: VisionData = {
        wallDist: [],
        goalDist: Number.MAX_VALUE,
        speed: 0,
        position: this.car.pos
    };
    private trackData: Track;
    private turnSpeed = 140;
    private carSprite: Phaser.GameObjects.Sprite;
    private lines: Phaser.Geom.Line[] = [];
    private lineGraphics: Phaser.GameObjects.Graphics;
    constructor(scene: Phaser.Scene, track: Track) {
        super(scene, 'Car');
        this.trackData = track;
        //create texture
        //init lines
        this.lineGraphics = this.scene.make.graphics({}, true);
        for (let i = 0; i < 9; i++) {
            this.lines.push(new Phaser.Geom.Line());
        }
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
            key: 'car',
            x: this.car.pos.y,
            y: this.car.pos.y
        }, true);
        //vision data
        this.updateVisionData();

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
    updateVisionData() {
        this.visionData.goalDist = Phaser.Geom.Line.Length(this.lines[GOAL]);
        this.visionData.position = this.car.pos;
        this.visionData.speed = this.car.vel.length();
        for (let i = 0; i < 8; i++) {
            this.visionData.wallDist[i] = Phaser.Geom.Line.Length(this.lines[i]);
        }
    }
    update(_dt: number): void {
        const { drive, turn } = this.carInput;
        this.car.rotation += turn * (this.car.vel.length()) * this.turnSpeed * _dt;
        this.car.rotation = this.car.rotation % 360;
        let theta = Math.DEG_TO_RAD * this.car.rotation;
        let dir = Math.Vector2.UP.clone().setAngle(theta)//new Math.Vector2(cos(theta), sin(theta));
        dir.normalize();
        let thrust = (drive < 0) ? -0.25 : drive;
        this.car.acc = dir.clone().scale(thrust * _dt * this.car.maxSpeed);

        this.car.vel.add(this.car.acc);
        this.car.pos.add(this.car.vel);

        this.car.vel.scale(this.car.dragCoefficient);

        this.car.vel.x = (abs(this.car.vel.x) <= 0.001) ? 0 : this.car.vel.x;
        this.car.vel.y = (abs(this.car.vel.y) <= 0.001) ? 0 : this.car.vel.y;

        this.carSprite.setPosition(this.car.pos.x, this.car.pos.y);
        this.carSprite.setAngle(this.car.rotation);

        let rect = this.carSprite.getBounds();
        this.lineGraphics.clear();

        for (let i = 0; i < 8; i++) {
            let angle = (i * Math.PI2) / 8;

            //let rectPoint = Phaser.Geom.Rectangle.PerimeterPoint(rect, angle );
            let hit = rayHit(this.car.pos, Math.Vector2.UP.clone().setAngle(angle), this.trackData.segments);
            this.lines[i].x1 = this.car.pos.x;
            this.lines[i].y1 = this.car.pos.y;
            this.lines[i].x2 = hit.x;
            this.lines[i].y2 = hit.y;
            this.lineGraphics
                .lineStyle(3, 0xFF0000)
                .strokeLineShape(this.lines[i]);
        }
        //GOAL LINE
        let rectPoint = Phaser.Geom.Rectangle.PerimeterPoint(rect, this.car.rotation);
        let hitPoint = rayHit(Math.Vector2.ZERO.clone().set(rectPoint.x, rectPoint.y), dir, this.trackData.checkpoints);

        this.lines[GOAL].x1 = rectPoint.x;
        this.lines[GOAL].y1 = rectPoint.y;
        this.lines[GOAL].x2 = hitPoint.x;
        this.lines[GOAL].y2 = hitPoint.y;
        this.lineGraphics
            .lineStyle(3, 0x0000FF)
            .strokeLineShape(this.lines[GOAL]);

    }

    checkOffTrack(): boolean {
        let isOffTrack = true;
        for (let i = 0; i < 4; i++) {
            let angle = (i * Math.PI2) / 4;

            let hits = rayHits(this.car.pos, Math.Vector2.UP.clone().setAngle(angle), this.trackData.segments);
            
            isOffTrack = isOffTrack && (hits.length % 2 == 0);

        }
        return isOffTrack;
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
