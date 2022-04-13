struct ColorVertex
{
    float4 Pos : POSITION;
};

struct ColorVertexOut
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR;
};

struct TransformData
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
    TransformData Data;
}

ColorVertexOut VtxColorShader(ColorVertex Vtx)
{
    ColorVertexOut NewOutPut = (ColorVertexOut) 0;
    NewOutPut.Pos = mul(Vtx.Pos, Data.WorldViewProjection);
    
    return NewOutPut;
}

float4 PixColorShader(ColorVertexOut VertexShaderOutput)	: SV_TARGET
{
    return float4(0.f, 1.f, 0.f, 1.f);

}