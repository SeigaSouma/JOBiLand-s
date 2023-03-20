xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 144;
 -96.22845;702.98602;-220.82312;,
 -48.11430;702.98602;-233.96822;,
 -48.11430;527.00623;-233.96822;,
 -96.22845;527.00623;-220.82312;,
 0.00000;702.98602;-247.11302;,
 0.00000;527.00623;-247.11302;,
 48.11430;702.98602;-260.25800;,
 48.11430;527.00623;-260.25800;,
 96.22845;702.98602;-273.40295;,
 96.22845;527.00623;-273.40295;,
 -48.11430;351.02628;-233.96822;,
 -96.22845;351.02628;-220.82312;,
 0.00000;351.02628;-247.11302;,
 48.11430;351.02628;-260.25800;,
 96.22845;351.02628;-273.40295;,
 -48.11430;175.04611;-233.96822;,
 -96.22845;175.04611;-220.82312;,
 0.00000;175.04611;-247.11302;,
 48.11430;175.04611;-260.25800;,
 96.22845;175.04611;-273.40295;,
 -48.11430;-0.93373;-233.96822;,
 -96.22845;-0.93373;-220.82312;,
 0.00000;-0.93373;-247.11302;,
 48.11430;-0.93373;-260.25800;,
 96.22845;-0.93373;-273.40295;,
 96.22845;702.98602;-273.40295;,
 99.39450;702.98602;-173.89172;,
 99.39450;527.00623;-173.89172;,
 96.22845;527.00623;-273.40295;,
 99.39450;702.98602;-114.90782;,
 99.39450;527.00623;-114.90782;,
 99.39450;702.98602;-53.69057;,
 99.39450;527.00623;-53.69057;,
 99.39450;702.98602;309.76422;,
 99.39450;527.00623;309.76422;,
 99.39450;351.02628;-173.89172;,
 96.22845;351.02628;-273.40295;,
 99.39450;351.02628;-114.90782;,
 99.39450;351.02628;-53.69057;,
 99.39450;351.02628;309.76422;,
 99.39450;175.04611;-173.89172;,
 96.22845;175.04611;-273.40295;,
 99.39450;175.04611;-114.90782;,
 99.39450;175.04611;-53.69057;,
 99.39450;175.04611;309.76422;,
 99.39450;-0.93373;-173.89172;,
 96.22845;-0.93373;-273.40295;,
 99.39450;-0.93373;-114.90782;,
 99.39450;-0.93373;-53.69057;,
 99.39450;-0.93373;309.76422;,
 99.39450;702.98602;309.76422;,
 49.69725;702.98602;309.76422;,
 49.69725;527.00623;309.76422;,
 99.39450;527.00623;309.76422;,
 0.00000;702.98602;309.76422;,
 0.00000;527.00623;309.76422;,
 -49.69725;702.98602;309.76422;,
 -49.69725;527.00623;309.76422;,
 -99.39450;702.98602;309.76422;,
 -99.39450;527.00623;309.76422;,
 49.69725;351.02628;309.76422;,
 99.39450;351.02628;309.76422;,
 0.00000;351.02628;309.76422;,
 -49.69725;351.02628;309.76422;,
 -99.39450;351.02628;309.76422;,
 49.69725;175.04611;309.76422;,
 99.39450;175.04611;309.76422;,
 0.00000;175.04611;309.76422;,
 -49.69725;175.04611;309.76422;,
 -99.39450;175.04611;309.76422;,
 49.69725;-0.93373;309.76422;,
 99.39450;-0.93373;309.76422;,
 0.00000;-0.93373;309.76422;,
 -49.69725;-0.93373;309.76422;,
 -99.39450;-0.93373;309.76422;,
 -99.39450;702.98602;309.76422;,
 -99.39450;702.98602;-53.69057;,
 -99.39450;527.00623;-53.69057;,
 -99.39450;527.00623;309.76422;,
 -99.39450;702.98602;-114.90782;,
 -99.39450;527.00623;-114.90782;,
 -99.39450;702.98602;-173.89172;,
 -99.39450;527.00623;-173.89172;,
 -96.22845;702.98602;-220.82312;,
 -96.22845;527.00623;-220.82312;,
 -99.39450;351.02628;-53.69057;,
 -99.39450;351.02628;309.76422;,
 -99.39450;351.02628;-114.90782;,
 -99.39450;351.02628;-173.89172;,
 -96.22845;351.02628;-220.82312;,
 -99.39450;175.04611;-53.69057;,
 -99.39450;175.04611;309.76422;,
 -99.39450;175.04611;-114.90782;,
 -99.39450;175.04611;-173.89172;,
 -96.22845;175.04611;-220.82312;,
 -99.39450;-0.93373;-53.69057;,
 -99.39450;-0.93373;309.76422;,
 -99.39450;-0.93373;-114.90782;,
 -99.39450;-0.93373;-173.89172;,
 -96.22845;-0.93373;-220.82312;,
 -49.69725;702.98602;309.76422;,
 -49.69725;702.98602;-53.69057;,
 -99.39450;702.98602;-53.69057;,
 0.00000;702.98602;-53.69057;,
 49.69725;702.98602;309.76422;,
 49.69725;702.98602;-53.69057;,
 99.39450;702.98602;-53.69057;,
 -49.69725;702.98602;-114.90782;,
 -99.39450;702.98602;-114.90782;,
 0.00000;702.98602;-114.90782;,
 49.69725;702.98602;-114.90782;,
 99.39450;702.98602;-114.90782;,
 -49.69725;702.98602;-173.89172;,
 -99.39450;702.98602;-173.89172;,
 0.00000;702.98602;-173.89172;,
 49.69725;702.98602;-173.89172;,
 99.39450;702.98602;-173.89172;,
 -48.11430;702.98602;-233.96822;,
 -96.22845;702.98602;-220.82312;,
 0.00000;702.98602;-247.11302;,
 48.11430;702.98602;-260.25800;,
 96.22845;702.98602;-273.40295;,
 -99.39450;-0.93373;-53.69057;,
 -49.69725;-0.93373;-53.69057;,
 -49.69725;-0.93373;309.76422;,
 0.00000;-0.93373;-53.69057;,
 49.69725;-0.93373;-53.69057;,
 49.69725;-0.93373;309.76422;,
 99.39450;-0.93373;-53.69057;,
 -99.39450;-0.93373;-114.90782;,
 -49.69725;-0.93373;-114.90782;,
 0.00000;-0.93373;-114.90782;,
 49.69725;-0.93373;-114.90782;,
 99.39450;-0.93373;-114.90782;,
 -99.39450;-0.93373;-173.89172;,
 -49.69725;-0.93373;-173.89172;,
 0.00000;-0.93373;-173.89172;,
 49.69725;-0.93373;-173.89172;,
 99.39450;-0.93373;-173.89172;,
 -96.22845;-0.93373;-220.82312;,
 -48.11430;-0.93373;-233.96822;,
 0.00000;-0.93373;-247.11302;,
 48.11430;-0.93373;-260.25800;,
 96.22845;-0.93373;-273.40295;;
 
 96;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;3,2,10,11;,
 4;2,5,12,10;,
 4;5,7,13,12;,
 4;7,9,14,13;,
 4;11,10,15,16;,
 4;10,12,17,15;,
 4;12,13,18,17;,
 4;13,14,19,18;,
 4;16,15,20,21;,
 4;15,17,22,20;,
 4;17,18,23,22;,
 4;18,19,24,23;,
 4;25,26,27,28;,
 4;26,29,30,27;,
 4;29,31,32,30;,
 4;31,33,34,32;,
 4;28,27,35,36;,
 4;27,30,37,35;,
 4;30,32,38,37;,
 4;32,34,39,38;,
 4;36,35,40,41;,
 4;35,37,42,40;,
 4;37,38,43,42;,
 4;38,39,44,43;,
 4;41,40,45,46;,
 4;40,42,47,45;,
 4;42,43,48,47;,
 4;43,44,49,48;,
 4;50,51,52,53;,
 4;51,54,55,52;,
 4;54,56,57,55;,
 4;56,58,59,57;,
 4;53,52,60,61;,
 4;52,55,62,60;,
 4;55,57,63,62;,
 4;57,59,64,63;,
 4;61,60,65,66;,
 4;60,62,67,65;,
 4;62,63,68,67;,
 4;63,64,69,68;,
 4;66,65,70,71;,
 4;65,67,72,70;,
 4;67,68,73,72;,
 4;68,69,74,73;,
 4;75,76,77,78;,
 4;76,79,80,77;,
 4;79,81,82,80;,
 4;81,83,84,82;,
 4;78,77,85,86;,
 4;77,80,87,85;,
 4;80,82,88,87;,
 4;82,84,89,88;,
 4;86,85,90,91;,
 4;85,87,92,90;,
 4;87,88,93,92;,
 4;88,89,94,93;,
 4;91,90,95,96;,
 4;90,92,97,95;,
 4;92,93,98,97;,
 4;93,94,99,98;,
 4;75,100,101,102;,
 4;100,54,103,101;,
 4;54,104,105,103;,
 4;104,33,106,105;,
 4;102,101,107,108;,
 4;101,103,109,107;,
 4;103,105,110,109;,
 4;105,106,111,110;,
 4;108,107,112,113;,
 4;107,109,114,112;,
 4;109,110,115,114;,
 4;110,111,116,115;,
 4;113,112,117,118;,
 4;112,114,119,117;,
 4;114,115,120,119;,
 4;115,116,121,120;,
 4;122,123,124,96;,
 4;123,125,72,124;,
 4;125,126,127,72;,
 4;126,128,49,127;,
 4;129,130,123,122;,
 4;130,131,125,123;,
 4;131,132,126,125;,
 4;132,133,128,126;,
 4;134,135,130,129;,
 4;135,136,131,130;,
 4;136,137,132,131;,
 4;137,138,133,132;,
 4;139,140,135,134;,
 4;140,141,136,135;,
 4;141,142,137,136;,
 4;142,143,138,137;;
 
 MeshMaterialList {
  1;
  96;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.708800;0.743200;0.743200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  22;
  -0.263548;0.000000;-0.964646;,
  -0.263544;0.000000;-0.964647;,
  -0.263543;0.000000;-0.964648;,
  -0.263545;0.000000;-0.964647;,
  -0.263545;0.000000;-0.964647;,
  -0.263545;0.000000;-0.964647;,
  -0.263548;0.000000;-0.964646;,
  -0.263545;0.000000;-0.964647;,
  -0.263545;0.000000;-0.964647;,
  0.999874;0.000000;-0.015902;,
  1.000000;0.000000;0.000000;,
  -0.263545;0.000000;-0.964647;,
  0.999874;0.000000;-0.015902;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  -0.999433;0.000000;-0.033673;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.999494;0.000000;-0.031800;,
  0.999494;0.000000;-0.031800;,
  -0.997732;0.000000;-0.067308;,
  -0.997732;0.000000;-0.067308;;
  96;
  4;0,1,1,0;,
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,8,11,3;,
  4;0,1,4,0;,
  4;1,2,2,4;,
  4;2,3,5,2;,
  4;3,11,11,5;,
  4;0,4,4,6;,
  4;4,2,2,4;,
  4;2,5,5,2;,
  4;5,11,11,5;,
  4;6,4,7,6;,
  4;4,2,2,7;,
  4;2,5,5,2;,
  4;5,11,11,5;,
  4;18,9,12,19;,
  4;9,10,10,12;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;19,12,12,19;,
  4;12,10,10,12;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;19,12,9,18;,
  4;12,10,10,9;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;18,9,9,18;,
  4;9,10,10,9;,
  4;10,10,10,10;,
  4;10,10,10,10;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;13,13,13,13;,
  4;14,14,14,14;,
  4;14,14,14,14;,
  4;14,15,15,14;,
  4;15,20,20,15;,
  4;14,14,14,14;,
  4;14,14,14,14;,
  4;14,15,15,14;,
  4;15,20,21,15;,
  4;14,14,14,14;,
  4;14,14,14,14;,
  4;14,15,15,14;,
  4;15,21,21,15;,
  4;14,14,14,14;,
  4;14,14,14,14;,
  4;14,15,15,14;,
  4;15,21,20,15;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;,
  4;17,17,17,17;;
 }
 MeshTextureCoords {
  144;
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.000000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;0.250000;,
  0.000000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.000000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.250000;0.500000;,
  0.000000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.250000;0.750000;,
  0.000000;0.750000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  1.000000;0.750000;,
  0.250000;1.000000;,
  0.000000;1.000000;,
  0.500000;1.000000;,
  0.750000;1.000000;,
  1.000000;1.000000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.250000;1.000000;,
  0.500000;0.750000;,
  0.750000;0.750000;,
  0.750000;1.000000;,
  1.000000;0.750000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.500000;0.500000;,
  0.750000;0.500000;,
  1.000000;0.500000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.500000;0.250000;,
  0.750000;0.250000;,
  1.000000;0.250000;,
  0.000000;0.000000;,
  0.250000;0.000000;,
  0.500000;0.000000;,
  0.750000;0.000000;,
  1.000000;0.000000;;
 }
}