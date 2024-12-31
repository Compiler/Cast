#version 460 core

layout (location = 0) in vec4 oColor;
layout (location = 1) in vec4 oTexCoords;

layout (location = 0) out vec4 FragColor;

uniform float u_time;
uniform sampler2D t_tex;


void main(){

    //FragColor = vec4(oColor.r, u_time * sin(oColor.g), oColor.b, 1.0f);
    //
    //FragColor = vec4(texture(u_texture1, oTexCoords).xyz * oColor, 1.0f); 
    vec4 col = vec4(oColor.rgb, 1.0f);
    vec4 texCol = texture(t_tex, oTexCoords.xy);

    float dist = 1.0;
    float distFromTop = gl_FragCoord.y;
    float smth = smoothstep(0, 180, distFromTop);
    if(gl_FragCoord.y < 90.5) dist = 0.5;
    dist = smth;
    
    //dist = 1.0f; // reset it until we have logic for the shadowing
    //FragColor = vec4(col.xyz * dist, col.a);  
    FragColor = vec4(texCol.xyz, col.a);  
}
