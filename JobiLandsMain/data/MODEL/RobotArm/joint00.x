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
 115;
 1.85347;-6.46705;-1.98383;,
 -0.05897;-6.46705;-2.73717;,
 -0.09871;-4.94578;-5.01355;,
 3.43502;-4.94578;-3.62157;,
 -1.94398;-6.46705;-1.91753;,
 -3.58173;-4.94578;-3.49911;,
 -2.69729;-6.46705;-0.00510;,
 -4.97365;-4.94578;0.03463;,
 -1.87769;-6.46705;1.87988;,
 -3.45924;-4.94578;3.51764;,
 0.03476;-6.46705;2.63323;,
 0.07449;-4.94578;4.90964;,
 1.91976;-6.46705;1.81360;,
 3.55750;-4.94578;3.39517;,
 2.67306;-6.46705;-0.09884;,
 4.94947;-4.94578;-0.13859;,
 1.85347;-6.46705;-1.98383;,
 3.43502;-4.94578;-3.62157;,
 -0.12528;-2.66905;-6.53461;,
 4.49179;-2.66905;-4.71589;,
 -4.67601;-2.66905;-4.55585;,
 -6.49464;-2.66905;0.06118;,
 -4.51595;-2.66905;4.61195;,
 0.10102;-2.66905;6.43068;,
 4.65181;-2.66905;4.45192;,
 6.47053;-2.66905;-0.16511;,
 4.49179;-2.66905;-4.71589;,
 -0.13459;0.01655;-7.06870;,
 4.86287;0.01655;-5.10016;,
 -5.06026;0.01655;-4.92694;,
 -7.02876;0.01655;0.07049;,
 -4.88702;0.01655;4.99622;,
 0.11036;0.01655;6.96478;,
 5.03609;0.01655;4.82301;,
 7.00466;0.01655;-0.17445;,
 4.86287;0.01655;-5.10016;,
 -0.12528;2.70214;-6.53461;,
 4.49179;2.70214;-4.71589;,
 -4.67601;2.70214;-4.55585;,
 -6.49464;2.70214;0.06118;,
 -4.51595;2.70214;4.61195;,
 0.10102;2.70214;6.43068;,
 4.65181;2.70214;4.45192;,
 6.47053;2.70214;-0.16511;,
 4.49179;2.70214;-4.71589;,
 -0.09871;4.97888;-5.01355;,
 3.43502;4.97888;-3.62157;,
 -3.58173;4.97888;-3.49911;,
 -4.97365;4.97888;0.03463;,
 -3.45924;4.97888;3.51764;,
 0.07449;4.97888;4.90964;,
 3.55750;4.97888;3.39517;,
 4.94947;4.97888;-0.13859;,
 3.43502;4.97888;-3.62157;,
 -0.05897;6.50014;-2.73717;,
 1.85347;6.50014;-1.98383;,
 -1.94398;6.50014;-1.91753;,
 -2.69729;6.50014;-0.00510;,
 -1.87769;6.50014;1.87988;,
 0.03476;6.50014;2.63323;,
 1.91976;6.50014;1.81360;,
 2.67306;6.50014;-0.09884;,
 1.85347;6.50014;-1.98383;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;-7.00125;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.01213;7.03434;-0.05197;,
 -0.98674;-5.38666;-4.56325;,
 1.91883;-1.57701;-6.60776;,
 45.62494;-35.32027;-7.37062;,
 42.71939;-39.12992;-5.32613;,
 4.89396;2.23264;-4.66591;,
 48.60006;-31.51062;-5.42878;,
 6.19590;3.81065;0.12477;,
 49.90197;-29.93261;-0.63814;,
 5.06196;2.23264;4.95797;,
 48.76801;-31.51062;4.19504;,
 2.15639;-1.57701;7.00248;,
 45.86250;-35.32027;6.23952;,
 -0.81875;-5.38666;5.06062;,
 42.88738;-39.12992;4.29771;,
 -2.12066;-6.96466;0.26993;,
 41.58544;-40.70793;-0.49295;,
 -0.98674;-5.38666;-4.56325;,
 42.71939;-39.12992;-5.32613;,
 1.91883;-1.57701;-6.60776;,
 -0.98674;-5.38666;-4.56325;,
 2.02259;-1.63235;0.19763;,
 4.89396;2.23264;-4.66591;,
 6.19590;3.81065;0.12477;,
 5.06196;2.23264;4.95797;,
 2.15639;-1.57701;7.00248;,
 -0.81875;-5.38666;5.06062;,
 -2.12066;-6.96466;0.26993;,
 42.71939;-39.12992;-5.32613;,
 45.62494;-35.32027;-7.37062;,
 40.39001;-31.18694;-0.47209;,
 48.60006;-31.51062;-5.42878;,
 49.90197;-29.93261;-0.63814;,
 48.76801;-31.51062;4.19504;,
 45.86250;-35.32027;6.23952;,
 42.88738;-39.12992;4.29771;,
 41.58544;-40.70793;-0.49295;;
 
 88;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;3,2,18,19;,
 4;2,5,20,18;,
 4;5,7,21,20;,
 4;7,9,22,21;,
 4;9,11,23,22;,
 4;11,13,24,23;,
 4;13,15,25,24;,
 4;15,17,26,25;,
 4;19,18,27,28;,
 4;18,20,29,27;,
 4;20,21,30,29;,
 4;21,22,31,30;,
 4;22,23,32,31;,
 4;23,24,33,32;,
 4;24,25,34,33;,
 4;25,26,35,34;,
 4;28,27,36,37;,
 4;27,29,38,36;,
 4;29,30,39,38;,
 4;30,31,40,39;,
 4;31,32,41,40;,
 4;32,33,42,41;,
 4;33,34,43,42;,
 4;34,35,44,43;,
 4;37,36,45,46;,
 4;36,38,47,45;,
 4;38,39,48,47;,
 4;39,40,49,48;,
 4;40,41,50,49;,
 4;41,42,51,50;,
 4;42,43,52,51;,
 4;43,44,53,52;,
 4;46,45,54,55;,
 4;45,47,56,54;,
 4;47,48,57,56;,
 4;48,49,58,57;,
 4;49,50,59,58;,
 4;50,51,60,59;,
 4;51,52,61,60;,
 4;52,53,62,61;,
 3;1,0,63;,
 3;4,1,64;,
 3;6,4,65;,
 3;8,6,66;,
 3;10,8,67;,
 3;12,10,68;,
 3;14,12,69;,
 3;16,14,70;,
 3;55,54,71;,
 3;54,56,72;,
 3;56,57,73;,
 3;57,58,74;,
 3;58,59,75;,
 3;59,60,76;,
 3;60,61,77;,
 3;61,62,78;,
 4;79,80,81,82;,
 4;80,83,84,81;,
 4;83,85,86,84;,
 4;85,87,88,86;,
 4;87,89,90,88;,
 4;89,91,92,90;,
 4;91,93,94,92;,
 4;93,95,96,94;,
 3;97,98,99;,
 3;100,97,99;,
 3;101,100,99;,
 3;102,101,99;,
 3;103,102,99;,
 3;104,103,99;,
 3;105,104,99;,
 3;98,105,99;,
 3;106,107,108;,
 3;107,109,108;,
 3;109,110,108;,
 3;110,111,108;,
 3;111,112,108;,
 3;112,113,108;,
 3;113,114,108;,
 3;114,106,108;;
 
 MeshMaterialList {
  1;
  88;
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
   0.400000;0.400000;0.400000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  85;
  0.264396;-0.924735;-0.273787;,
  -0.006642;-0.924735;-0.380553;,
  -0.273790;-0.924735;-0.264393;,
  -0.380556;-0.924734;0.006643;,
  -0.264397;-0.924735;0.273787;,
  0.006642;-0.924736;0.380551;,
  0.273790;-0.924736;0.264392;,
  0.380554;-0.924735;-0.006643;,
  0.489718;-0.709232;-0.507116;,
  -0.012303;-0.709232;-0.704868;,
  -0.507122;-0.709229;-0.489715;,
  -0.704876;-0.709224;0.012305;,
  -0.489724;-0.709228;0.507115;,
  0.012302;-0.709232;0.704868;,
  0.507118;-0.709232;0.489715;,
  0.704868;-0.709232;-0.012305;,
  0.641240;-0.384553;-0.664025;,
  -0.016112;-0.384551;-0.922963;,
  -0.664031;-0.384548;-0.641237;,
  -0.922966;-0.384543;0.016113;,
  -0.641249;-0.384545;0.664021;,
  0.016107;-0.384549;0.922964;,
  0.664025;-0.384553;0.641241;,
  0.922961;-0.384556;-0.016111;,
  0.694657;0.000000;-0.719341;,
  -0.017455;0.000000;-0.999848;,
  -0.719345;0.000000;-0.694653;,
  -0.999848;0.000000;0.017455;,
  -0.694663;0.000000;0.719335;,
  0.017448;0.000000;0.999848;,
  0.719339;0.000000;0.694659;,
  0.999848;0.000000;-0.017452;,
  0.641240;0.384553;-0.664026;,
  -0.016112;0.384550;-0.922964;,
  -0.664032;0.384547;-0.641237;,
  -0.922967;0.384543;0.016113;,
  -0.641249;0.384544;0.664022;,
  0.016107;0.384549;0.922964;,
  0.664025;0.384552;0.641242;,
  0.922962;0.384555;-0.016111;,
  0.489718;0.709232;-0.507116;,
  -0.012303;0.709231;-0.704868;,
  -0.507122;0.709230;-0.489715;,
  -0.704876;0.709224;0.012305;,
  -0.489724;0.709228;0.507115;,
  0.012302;0.709232;0.704867;,
  0.507118;0.709233;0.489715;,
  0.704867;0.709233;-0.012305;,
  0.264395;0.924736;-0.273786;,
  -0.006642;0.924736;-0.380552;,
  -0.273789;0.924736;-0.264392;,
  -0.380554;0.924735;0.006643;,
  -0.264396;0.924736;0.273786;,
  0.006642;0.924736;0.380550;,
  0.273788;0.924737;0.264390;,
  0.380552;0.924736;-0.006644;,
  -0.000001;-1.000000;-0.000000;,
  -0.000001;1.000000;-0.000000;,
  -0.444355;-0.559739;-0.699458;,
  -0.017452;-0.000000;-0.999848;,
  0.419674;0.559739;-0.714540;,
  0.610960;0.791589;-0.010664;,
  0.444356;0.559738;0.699458;,
  0.017454;0.000001;0.999848;,
  -0.419673;-0.559739;0.714540;,
  -0.610960;-0.791590;0.010664;,
  -0.791459;0.611066;0.013815;,
  0.406255;-0.821781;0.399541;,
  -0.791405;0.611055;0.017035;,
  -0.790023;0.612866;0.016079;,
  -0.792808;0.609259;0.016107;,
  -0.793417;0.608521;0.013851;,
  -0.792885;0.609261;0.011573;,
  -0.791517;0.611055;0.010594;,
  -0.790104;0.612865;0.011502;,
  -0.789481;0.613620;0.013781;,
  0.874611;-0.038838;0.483266;,
  0.573679;-0.433410;0.695017;,
  0.265540;-0.827982;0.493897;,
  0.130698;-0.991420;-0.002281;,
  0.248142;-0.827982;-0.502864;,
  0.549073;-0.433409;-0.714615;,
  0.888858;-0.178110;-0.422147;,
  0.857212;-0.038840;-0.513496;,
  0.992056;0.124598;-0.017316;;
  88;
  4;0,1,9,8;,
  4;1,2,10,9;,
  4;2,3,11,10;,
  4;3,4,12,11;,
  4;4,5,13,12;,
  4;5,6,14,13;,
  4;6,7,15,14;,
  4;7,0,8,15;,
  4;8,9,17,16;,
  4;9,10,18,17;,
  4;10,11,19,18;,
  4;11,12,20,19;,
  4;12,13,21,20;,
  4;13,14,22,21;,
  4;14,15,23,22;,
  4;15,8,16,23;,
  4;16,17,25,24;,
  4;17,18,26,25;,
  4;18,19,27,26;,
  4;19,20,28,27;,
  4;20,21,29,28;,
  4;21,22,30,29;,
  4;22,23,31,30;,
  4;23,16,24,31;,
  4;24,25,33,32;,
  4;25,26,34,33;,
  4;26,27,35,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,24,32,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,35,43,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,32,40,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,43,51,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,40,48,55;,
  3;1,0,56;,
  3;2,1,56;,
  3;3,2,56;,
  3;4,3,56;,
  3;5,4,56;,
  3;6,5,56;,
  3;7,6,56;,
  3;0,7,56;,
  3;48,49,57;,
  3;49,50,57;,
  3;50,51,57;,
  3;51,52,57;,
  3;52,53,57;,
  3;53,54,57;,
  3;54,55,57;,
  3;55,48,57;,
  4;58,59,59,58;,
  4;59,60,60,59;,
  4;60,61,61,60;,
  4;61,62,62,61;,
  4;62,63,63,62;,
  4;63,64,64,63;,
  4;64,65,65,64;,
  4;65,58,58,65;,
  3;68,69,66;,
  3;70,68,66;,
  3;71,70,66;,
  3;72,71,66;,
  3;73,72,66;,
  3;74,73,66;,
  3;75,74,66;,
  3;69,75,66;,
  3;76,77,67;,
  3;77,78,67;,
  3;78,79,67;,
  3;79,80,67;,
  3;80,81,82;,
  3;81,83,82;,
  3;83,84,82;,
  3;84,76,82;;
 }
 MeshTextureCoords {
  115;
  0.000000;0.875000;,
  0.125000;0.875000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.875000;,
  0.250000;0.750000;,
  0.375000;0.875000;,
  0.375000;0.750000;,
  0.500000;0.875000;,
  0.500000;0.750000;,
  0.625000;0.875000;,
  0.625000;0.750000;,
  0.750000;0.875000;,
  0.750000;0.750000;,
  0.875000;0.875000;,
  0.875000;0.750000;,
  1.000000;0.875000;,
  1.000000;0.750000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.250000;0.125000;,
  0.375000;0.125000;,
  0.500000;0.125000;,
  0.625000;0.125000;,
  0.750000;0.125000;,
  0.875000;0.125000;,
  1.000000;0.125000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.375000;0.687500;,
  0.406250;0.687500;,
  0.406250;0.311560;,
  0.375000;0.311560;,
  0.437500;0.687500;,
  0.437500;0.311560;,
  0.468750;0.687500;,
  0.468750;0.311560;,
  0.500000;0.687500;,
  0.500000;0.311560;,
  0.531250;0.687500;,
  0.531250;0.311560;,
  0.562500;0.687500;,
  0.562500;0.311560;,
  0.593750;0.687500;,
  0.593750;0.311560;,
  0.625000;0.687500;,
  0.625000;0.311560;,
  0.500000;1.000000;,
  0.610485;0.954235;,
  0.500000;0.850000;,
  0.389515;0.954235;,
  0.343750;0.843750;,
  0.389515;0.733265;,
  0.500000;0.687500;,
  0.610485;0.733265;,
  0.656250;0.843750;,
  0.610485;0.045765;,
  0.500000;0.000000;,
  0.500000;0.162500;,
  0.389515;0.045765;,
  0.343750;0.156250;,
  0.389515;0.266735;,
  0.500000;0.312500;,
  0.610485;0.266735;,
  0.656250;0.156250;;
 }
}