#version 330 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 18) out;

in vec4 fragCurrPos[];
in vec4 fragPrevPos[];

out vec4 gFragCurrPos;
out vec4 gFragPrevPos;

struct Candidate {
    vec2 pos;
    int index;
    float t;
    float angle;
};

const int NUM_CANDIDATES = 6;

vec4 convertToClip(vec2 pos, float z) {
    return vec4(pos * 2.0 - 1.0, z, 1.0);
}

void main() {
    vec2 current[3];
    vec2 previous[3];
    vec2 motion[3];
    
    Candidate candidates[NUM_CANDIDATES];
    
    for (int i = 0; i < 3; i++) {
        vec2 curr_ndc = (fragCurrPos[i].xy / fragCurrPos[i].w) * 0.5 + 0.5;
        vec2 prev_ndc = (fragPrevPos[i].xy / fragPrevPos[i].w) * 0.5 + 0.5;
        current[i] = curr_ndc;
        previous[i] = prev_ndc;
        motion[i] = curr_ndc - prev_ndc;
        
        candidates[i].pos = curr_ndc;
        candidates[i].index = i;
        candidates[i].t = 1.0;
        candidates[i].angle = 0.0;

        candidates[i+3].pos = prev_ndc;
        candidates[i+3].index = i;
        candidates[i+3].t = 0.0;
        candidates[i+3].angle = 0.0;
    }
    
    vec2 centerAvg = vec2(0.0);
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        centerAvg += candidates[i].pos;
    }
    centerAvg /= float(NUM_CANDIDATES);
    
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        vec2 diff = candidates[i].pos - centerAvg;
        candidates[i].angle = atan(diff.y, diff.x);
    }
    
    for (int i = 0; i < NUM_CANDIDATES - 1; i++) {
        for (int j = 0; j < NUM_CANDIDATES - i - 1; j++) {
            if (candidates[j].angle > candidates[j+1].angle) {
                Candidate temp = candidates[j];
                candidates[j] = candidates[j+1];
                candidates[j+1] = temp;
            }
        }
    }
    
    Candidate hull[NUM_CANDIDATES];
    int hullSize = 0;
    for (int i = 0; i < NUM_CANDIDATES; i++) {
        while (hullSize >= 2) {
            Candidate a = hull[hullSize - 2];
            Candidate b = hull[hullSize - 1];
            Candidate c = candidates[i];
            vec2 ab = b.pos - a.pos;
            vec2 ac = c.pos - a.pos;
            if (ab.x * ac.y - ab.y * ac.x <= 0.0)
                hullSize--;
            else
                break;
        }
        hull[hullSize] = candidates[i];
        hullSize++;
    }
    
    vec2 fanCenter = vec2(0.0);
    for (int i = 0; i < hullSize; i++) {
        fanCenter += hull[i].pos;
    }
    fanCenter /= float(hullSize);
    
    vec2 avgMotion = vec2(0.0);
    for (int i = 0; i < hullSize; i++) {
        int idx = hull[i].index;
        avgMotion += motion[idx];
    }
    avgMotion /= float(hullSize);
    
    vec2 center_prev = fanCenter;
    vec2 center_curr = fanCenter + avgMotion;

    float depth = fragCurrPos[0].z;
    
    for (int i = 0; i < hullSize; i++) {
        int next = (i + 1) % hullSize;

        vec4 centerCurrClip = convertToClip(center_curr, depth);
        vec4 centerPrevClip = convertToClip(center_prev, depth);
        gFragCurrPos = centerCurrClip;
        gFragPrevPos = centerPrevClip;
        gl_Position = centerCurrClip;
        EmitVertex();
        
        int idx = hull[i].index;
        vec2 candPos = hull[i].pos;
        vec2 candMotion = motion[idx];
        vec2 curr_out;
        vec2 prev_out;
        if (hull[i].t > 0.5) {
            curr_out = candPos;
            prev_out = candPos - candMotion;
        } else {
            prev_out = candPos;
            curr_out = candPos + candMotion;
        }
        vec4 currClip_i = convertToClip(curr_out, depth);
        vec4 prevClip_i = convertToClip(prev_out, depth);
        gFragCurrPos = currClip_i;
        gFragPrevPos = prevClip_i;
        gl_Position = currClip_i;
        EmitVertex();
        
        idx = hull[next].index;
        candPos = hull[next].pos;
        candMotion = motion[idx];
        if (hull[next].t > 0.5) {
            curr_out = candPos;
            prev_out = candPos - candMotion;
        } else {
            prev_out = candPos;
            curr_out = candPos + candMotion;
        }
        vec4 currClip_next = convertToClip(curr_out, depth);
        vec4 prevClip_next = convertToClip(prev_out, depth);
        gFragCurrPos = currClip_next;
        gFragPrevPos = prevClip_next;
        gl_Position = currClip_next;
        EmitVertex();
        
        EndPrimitive();
    }
}
