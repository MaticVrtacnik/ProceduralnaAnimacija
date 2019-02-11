
//CONSTS=======================================================================
/** Increase to make depth edges crisper. Decrease to reduce flicker. */
#define EDGE_SHARPNESS     (0.075)

/** Step in 2-pixel intervals since we already blurred against neighbors in the
    first AO pass.  This constant can be increased while R decreases to improve
    performance at the expense of some dithering artifacts. 
    
    Morgan found that a scale of 3 left a 1-pixel checkerboard grid that was
    unobjectionable after shading was applied but eliminated most temporal incoherence
    from using small numbers of sample taps.
    */
#define SCALE               (1)

/** Filter radius in pixels. This will be multiplied by SCALE. */
#define R                   (6)


const float gaussian[R + 1] = 
//  float[](0.356642, 0.239400, 0.072410, 0.009869);
//  float[](0.398943, 0.241971, 0.053991, 0.004432, 0.000134);  // stddev = 1.0
	//float[](0.153170, 0.144893, 0.122649, 0.092902, 0.062970);  // stddev = 2.0
    float[](0.111220, 0.107798, 0.098151, 0.083953, 0.067458, 0.050920, 0.036108); // stddev = 3.0


//INPUTS=======================================================================
in block{
	vec2 uv;
}IN;

//TEXTURES=====================================================================
layout (binding = 0) uniform sampler2D u_saoDepthTexture;

//UNIFORMS=====================================================================
uniform ivec2 u_direction;

//OUTPUTS======================================================================
layout (location = 0) out vec3 o_fragColor;


//CODE=========================================================================
float unpackKey(vec2 p);

void main()
{
	ivec2 uv = ivec2(gl_FragCoord.xy);
	
	vec3 saoDepth = texelFetch(u_saoDepthTexture, uv, 0).rgb;
    o_fragColor.bg = saoDepth.bg;
	
    float key = unpackKey(o_fragColor.bg);
   
    float sum = saoDepth.r;

    if (key == 1.0) { 
        // Sky pixel (if you aren't using depth keying, disable this test)
        o_fragColor.r = sum;
        return;
    }
	
	
	
	 // Base weight for depth falloff.  Increase this for more blurriness,
    // decrease it for better edge discrimination
    float BASE = gaussian[0];
    float totalWeight = BASE;
    sum *= totalWeight;

   
    for (int r = -R; r <= R; ++r) {
        // We already handled the zero case above.  This loop should be unrolled and the static branch optimized out,
        // so the IF statement has no runtime cost
        if (r != 0) {
            saoDepth = texelFetch(u_saoDepthTexture, uv + u_direction * (r * SCALE), 0).rgb;
            float value  = saoDepth.r;
			
			
			float tapKey = unpackKey(saoDepth.bg);
			//float tapKey = texelFetch(u_saoDepthTexture, uv + u_direction * (r * SCALE), 0).w;
            
            // spatial domain: offset gaussian tap
            float weight = 0.3 + gaussian[abs(r)];
            
            // range domain (the "bilateral" weight). As depth difference increases, decrease weight.
            weight *= max(0.0, 1.0 - (EDGE_SHARPNESS * 1000.0) * abs(tapKey - key));

            sum += value * weight;
            totalWeight += weight;
        }
    }
 
    const float epsilon = 0.0001;
    o_fragColor.r = sum / (totalWeight + epsilon);
	//o_fragColor.r = 0.0;

	
	//o_fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}


float unpackKey(vec2 p)
{
    return p.x * (256.0 / 257.0) + p.y * (1.0 / 257.0);
}
