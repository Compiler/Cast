#version 460 core

layout (location = 0) in vec4 oColor;
layout (location = 1) in vec4 oTexCoords;

layout (location = 0) out vec4 FragColor;

uniform float u_time;
uniform sampler2D t_tex;

float plot(vec2 st) {    
    return smoothstep(0.02, 0.0, abs(st.y - st.x));
}
void main(){

    //FragColor = vec4(oColor.r, u_time * sin(oColor.g), oColor.b, 1.0f);
    //
    //FragColor = vec4(texture(u_texture1, oTexCoords).xyz * oColor, 1.0f); 
    vec4 col = vec4(oColor.rgb, 1.0f);
    vec4 texCol = texture(t_tex, oTexCoords.xy);

    vec2 curPos = gl_FragCoord.xy;
    float y = sin(u_time);
    float val = plot(curPos);

    vec3 mcol = (1.0-val)*col.xyz+val*vec3(0.0,1.0,0.0);


    //dist = 1.0f; // reset it until we have logic for the shadowing
    //FragColor = vec4(col.xyz * dist, col.a);  
    FragColor = mix(vec4(mcol, 1.0), vec4(texCol.xyz, col.a), 0.8);  
}
