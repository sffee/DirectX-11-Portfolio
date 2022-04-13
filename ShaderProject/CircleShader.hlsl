struct TexVertex
{
    float4 Pos : POSITION;
    float4 Uv : TEXCOORD;
};

struct TexVertexOut
{
    float4 Pos : SV_POSITION;
    float4 CutPos : POSITION0;
    float4 Uv : TEXCOORD;
};

struct TransFormData
{
    float4x4 Scale;
    float4x4 Rotation;
    float4x4 Position;
    float4x4 Revolve;
    float4x4 Parent;
    float4x4 LocalWorld;
    float4x4 World;
    float4x4 View;
    float4x4 Projection;
    float4x4 WorldView;
    float4x4 ViewProjection;
    float4x4 WorldViewProjection;
};

cbuffer RenderingTransformData : register(b0)
{
    TransFormData Data;
}

TexVertexOut CircleVtxShader(TexVertex Vtx)
{
    TexVertexOut NewOutPut = (TexVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, Data.WorldViewProjection);
    
    NewOutPut.CutPos = mul(Vtx.Pos, Data.World);
    
    NewOutPut.Uv.x = (Vtx.Uv.x - 0.5f) * 2.f;
    NewOutPut.Uv.y = (Vtx.Uv.y - 0.5f) * 2.f;
    
    return NewOutPut;
}

cbuffer RangeData : register(b1)
{
    float4 Range;
}

float4 CirclePixShader(TexVertexOut VertexShaderOutput) : SV_TARGET
{
    if (length(VertexShaderOutput.Uv) < 1.f)
    {
        VertexShaderOutput.Uv *= 2.f;
        
        float Rad = atan2(-VertexShaderOutput.Uv.y, VertexShaderOutput.Uv.x);
        float Deg = Rad * 180 / 3.14;
        
        if (-15.f < Deg && Deg < 15.f)
        {
            if (Range.x != 0.f)
            {
                if (VertexShaderOutput.CutPos.x < Range.z)
                {
                    return float4(1.f, 0.f, 0.f, 0.5f);
                }
            }
            else
            {
                return float4(1.f, 0.f, 0.f, 0.5f);
            }
        }
    }

    clip(-1);
    
    return float4(1.f, 0.f, 0.f, 1.f);
}