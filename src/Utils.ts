import { Math } from "phaser";
import { min } from 'mathjs'

export function rayHit(startPoint : Math.Vector2, dir : Math.Vector2, lines : Phaser.Geom.Line[]) : Math.Vector2 {
    let hit : Math.Vector2 = new Math.Vector2;
    let r = startPoint;
    let minT = Number.MAX_VALUE;
    lines.forEach(line => {
        let a = new Math.Vector2(line.x1, line.y1);
        let b = new Math.Vector2(line.x2, line.y2);

        let m = b.subtract(a);
        let t2 = (dir.x * (a.y - r.y) + dir.y * (r.x - a.x)) / (m.x * dir.y - m.y * dir.x);
        let t1 = (a.x + m.x * t2 - r.x) / dir.x;
        if(t1 >= 0 && t2 >= 0 && t2 <= 1){
            minT = min(minT, t1);
        }
    });
    hit = startPoint.clone().add(dir.clone().scale(minT));
    return hit;
}

export function rayHits(startPoint : Math.Vector2, dir : Math.Vector2, lines : Phaser.Geom.Line[]) : Math.Vector2[] {
    let hits : Math.Vector2[] = [];
    lines.forEach(line => {
        let a = new Math.Vector2(line.x1, line.y1);
        let b = new Math.Vector2(line.x2, line.y2);
        
        let r = startPoint.clone();
        let d = dir.clone();
        let m = b.subtract(a);
        let t2 = (dir.x * (a.y - r.y) + dir.y * (r.x - a.x)) / (m.x * dir.y - m.y * dir.x);
        let t1 = (a.x + m.x * t2 - r.x) / dir.x;
        if(t1 >= 0 && t2 >= 0 && t2 <= 1){
            hits.push(r.add(d.scale(t1)));
        }
    });
    return hits;
}