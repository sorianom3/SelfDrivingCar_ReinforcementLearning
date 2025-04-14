import { all, cos, create, random, sin } from 'mathjs';
import * as Phaser from 'phaser';
import {Math} from 'phaser';
 
 export default class Track extends Phaser.GameObjects.GameObject {
    checkpoints : Phaser.Geom.Line[] = [];
    segments: Phaser.Geom.Line[] = [];
    constructor(scene: Phaser.Scene){
        super(scene,"track");
        const math = create(all, {
            randomSeed: '0' 
          });
        //generate track points
        let w = window.innerWidth;
        let h = window.innerHeight;

        let radiusX = w * 0.30;
        let radiusY = h * 0.30;

        let checkPointCount = 12;
        let worldPoints = [];
        for(let i = 0; i < checkPointCount;i++){
            let theta = (i * Math.PI2) /checkPointCount;
            let irr = 0.57;

            let tempRX = radiusX + (math.random(-70,70) * irr * 2 - irr);
            let tempRY = radiusY + (math.random(-70,70) * irr * 2 - irr);

            let radX = cos(theta); 
            let radY = sin(theta); 

            worldPoints[i] = new Math.Vector2(
                tempRX * radX + (w * 0.5),
                tempRY * radY + (h * 0.5)
            );
            worldPoints[checkPointCount + i] =new Math.Vector2(
                (tempRX + 100) * radX + (w * 0.5),
                (tempRY + 100) * radY + (h * 0.5)
            );
            this.checkpoints[i] = new Phaser.Geom.Line(
                worldPoints[i].x,
                worldPoints[i].y,
                worldPoints[checkPointCount + i].x,
                worldPoints[checkPointCount + i].y
            );
        }

        for(let i = 0; i < checkPointCount * 2;i++){
            
            let k = i + 1;
            if(i == checkPointCount - 1){
                k = 0;
            } else if(i == checkPointCount*2 - 1){
                k = checkPointCount;
            }
            
            this.segments[i] = new Phaser.Geom.Line(
                worldPoints[i].x,
                worldPoints[i].y,
                worldPoints[k].x,
                worldPoints[k].y
            )
        }

        //create segment graphic
        let graphic1 = this.scene.make.graphics({},true);
        let graphic2 = this.scene.make.graphics({}, true);
        for (let i = 0; i < this.segments.length; i++) {
            const line = this.segments[i];
            graphic1
                .lineStyle(3,0xFFFFFF,1.0)
                .strokeLineShape(line);
        }
        for (let i = 0; i < this.checkpoints.length; i++) {
            const line = this.checkpoints[i];
            graphic2
                .lineStyle(3,0x00FF00,1.0)
                .strokeLineShape(line);
        }
    }

 }