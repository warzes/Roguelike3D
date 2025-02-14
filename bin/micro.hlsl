struct vertex { float4 pos : SV_POSITION; float4 col : COL; };

vertex vertex_shader(uint vid : SV_VERTEXID)
{ 
    vertex output = { float4(vid * 0.5f, vid & 1, 1, 1.5f) - 0.5f, float4(vid == 0, vid == 1, vid == 2, 1) };
    return output;
}

float4 pixel_shader(vertex input) : SV_TARGET 
{ 
    return input.col;
}