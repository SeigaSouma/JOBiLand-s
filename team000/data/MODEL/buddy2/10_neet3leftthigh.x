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
 134;
 1.66798;-18.54125;2.46320;,
 2.38974;-16.52641;2.65640;,
 0.03176;-16.52641;3.63311;,
 0.03176;-18.54125;2.89060;,
 -2.32620;-16.52641;2.65640;,
 -1.79159;-18.54125;2.49876;,
 -3.30289;-16.52641;0.29844;,
 -2.93079;-18.54125;0.41211;,
 -2.32620;-16.52641;-2.05953;,
 -2.19984;-18.54125;-1.83890;,
 0.03176;-16.52641;-3.03623;,
 -0.94907;-18.54125;-2.59437;,
 0.03176;-18.54125;-3.17670;,
 0.92824;-18.54125;-2.82190;,
 2.08878;-18.54125;-1.82118;,
 2.38974;-16.52641;-2.05953;,
 3.36644;-16.52641;0.29844;,
 2.95387;-18.54125;0.29844;,
 2.38974;-16.52641;2.65640;,
 1.66798;-18.54125;2.46320;,
 2.38974;-14.51158;2.65640;,
 0.03176;-14.51158;3.63311;,
 -2.32620;-14.51158;2.65640;,
 -3.30289;-14.51158;0.29844;,
 -2.32620;-14.51158;-2.05953;,
 0.03176;-14.51158;-3.03623;,
 2.38974;-14.51158;-2.05953;,
 3.36644;-14.51158;0.29844;,
 2.38974;-14.51158;2.65640;,
 2.38974;-12.49674;2.65640;,
 0.03176;-12.49674;3.63311;,
 -2.32620;-12.49674;2.65640;,
 -3.30289;-12.49674;0.29844;,
 -2.32620;-12.49674;-2.05953;,
 0.03176;-12.49674;-3.03623;,
 2.38974;-12.49674;-2.05953;,
 3.36644;-12.49674;0.29844;,
 2.38974;-12.49674;2.65640;,
 2.38974;-10.48190;2.65640;,
 0.03176;-10.48190;3.63311;,
 -2.32620;-10.48190;2.65640;,
 -3.30289;-10.48190;0.29844;,
 -2.32620;-10.48190;-2.05953;,
 0.03176;-10.48190;-3.03623;,
 2.38974;-10.48190;-2.05953;,
 3.36644;-10.48190;0.29844;,
 2.38974;-10.48190;2.65640;,
 0.03176;-18.54125;2.89060;,
 0.03176;-20.66652;-0.30648;,
 1.66798;-18.54125;2.46320;,
 -1.79159;-18.54125;2.49876;,
 -2.93079;-18.54125;0.41211;,
 -2.19984;-18.54125;-1.83890;,
 0.03176;-18.95366;-2.61618;,
 0.03176;-18.54125;-3.17670;,
 -0.94907;-18.54125;-2.59437;,
 2.08878;-18.54125;-1.82118;,
 0.92824;-18.54125;-2.82190;,
 2.95387;-18.54125;0.29844;,
 2.38974;-10.48190;2.65640;,
 0.03176;-10.48190;0.29844;,
 0.03176;-10.48190;3.63311;,
 -2.32620;-10.48190;2.65640;,
 -3.30289;-10.48190;0.29844;,
 -2.32620;-10.48190;-2.05953;,
 0.03176;-10.48190;-3.03623;,
 2.38974;-10.48190;-2.05953;,
 3.36644;-10.48190;0.29844;,
 2.73480;-11.75717;3.05790;,
 2.73480;-7.70367;2.97499;,
 0.00001;-7.68079;4.09356;,
 0.00001;-11.73430;4.17647;,
 -2.73480;-7.70367;2.97499;,
 -2.73480;-11.75717;3.05790;,
 -3.86758;-7.75890;0.27454;,
 -3.86758;-11.81241;0.35745;,
 -2.73480;-7.81414;-2.42592;,
 -2.73480;-11.86765;-2.34301;,
 0.00001;-7.83702;-3.54449;,
 0.00001;-11.89052;-3.46158;,
 2.73480;-7.81414;-2.42592;,
 2.73480;-11.86765;-2.34301;,
 3.86758;-7.75890;0.27454;,
 3.86758;-11.81241;0.35745;,
 2.73480;-7.70367;2.97499;,
 2.73480;-11.75717;3.05790;,
 2.73480;-6.72539;2.88630;,
 0.00001;-6.70251;4.00486;,
 -2.73480;-6.72539;2.88630;,
 -3.86758;-6.78062;0.18584;,
 -2.73480;-6.83586;-2.51462;,
 0.00001;-6.85874;-3.63319;,
 2.73480;-6.83586;-2.51462;,
 3.86758;-6.78062;0.18584;,
 2.73480;-6.72539;2.88630;,
 2.64527;-5.15248;2.98966;,
 0.00001;-5.12805;4.18443;,
 -2.64528;-5.15248;2.98966;,
 -3.85524;-5.21148;0.10523;,
 -2.64528;-5.27048;-2.77920;,
 0.00001;-5.29492;-3.97397;,
 2.64527;-5.27048;-2.77920;,
 3.85523;-5.21148;0.10523;,
 2.64527;-5.15248;2.98966;,
 3.49555;-0.38135;5.34252;,
 0.00001;-0.40603;6.73424;,
 -3.13316;-0.30828;5.16863;,
 -3.42646;-0.13557;0.01029;,
 -2.20092;1.52169;-2.50457;,
 0.00001;1.49425;-4.39410;,
 2.42989;1.52066;-3.10313;,
 3.42646;-0.53324;0.00954;,
 3.49555;-0.38135;5.34252;,
 0.00001;-11.73430;4.17647;,
 0.00001;-11.92564;-0.06384;,
 2.73480;-11.75717;3.05790;,
 -2.73480;-11.75717;3.05790;,
 -3.86758;-11.81241;0.35745;,
 -2.73480;-11.86765;-2.34301;,
 0.00001;-11.89052;-3.46158;,
 2.73480;-11.86765;-2.34301;,
 3.86758;-11.81241;0.35745;,
 3.49555;-0.38135;5.34252;,
 0.00001;4.17579;4.97997;,
 0.00001;-0.40603;6.73424;,
 -3.13316;-0.30828;5.16863;,
 -3.42646;-0.13557;0.01029;,
 -1.79927;3.14864;-1.20272;,
 -2.20092;1.52169;-2.50457;,
 0.00001;4.16151;-2.54232;,
 0.00001;1.49425;-4.39410;,
 2.42989;1.52066;-3.10313;,
 2.03347;3.19865;-1.47788;,
 3.42646;-0.53324;0.00954;;
 
 108;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 3;8,10,9;,
 3;9,10,11;,
 3;10,12,11;,
 3;12,10,13;,
 3;13,10,14;,
 3;10,15,14;,
 4;14,15,16,17;,
 4;17,16,18,19;,
 4;1,20,21,2;,
 4;2,21,22,4;,
 4;4,22,23,6;,
 4;6,23,24,8;,
 4;8,24,25,10;,
 4;10,25,26,15;,
 4;15,26,27,16;,
 4;16,27,28,18;,
 4;20,29,30,21;,
 4;21,30,31,22;,
 4;22,31,32,23;,
 4;23,32,33,24;,
 4;24,33,34,25;,
 4;25,34,35,26;,
 4;26,35,36,27;,
 4;27,36,37,28;,
 4;29,38,39,30;,
 4;30,39,40,31;,
 4;31,40,41,32;,
 4;32,41,42,33;,
 4;33,42,43,34;,
 4;34,43,44,35;,
 4;35,44,45,36;,
 4;36,45,46,37;,
 3;47,48,49;,
 3;50,48,47;,
 3;51,48,50;,
 3;52,48,51;,
 3;48,52,53;,
 3;53,52,54;,
 3;52,55,54;,
 3;56,48,57;,
 3;57,48,54;,
 3;48,53,54;,
 3;58,48,56;,
 3;49,48,58;,
 3;59,60,61;,
 3;61,60,62;,
 3;62,60,63;,
 3;63,60,64;,
 3;64,60,65;,
 3;65,60,66;,
 3;66,60,67;,
 3;67,60,59;,
 4;68,69,70,71;,
 4;71,70,72,73;,
 4;73,72,74,75;,
 4;75,74,76,77;,
 4;77,76,78,79;,
 4;79,78,80,81;,
 4;81,80,82,83;,
 4;83,82,84,85;,
 4;69,86,87,70;,
 4;70,87,88,72;,
 4;72,88,89,74;,
 4;74,89,90,76;,
 4;76,90,91,78;,
 4;78,91,92,80;,
 4;80,92,93,82;,
 4;82,93,94,84;,
 4;86,95,96,87;,
 4;87,96,97,88;,
 4;88,97,98,89;,
 4;89,98,99,90;,
 4;90,99,100,91;,
 4;91,100,101,92;,
 4;92,101,102,93;,
 4;93,102,103,94;,
 4;95,104,105,96;,
 4;96,105,106,97;,
 4;97,106,107,98;,
 4;98,107,108,99;,
 4;99,108,109,100;,
 4;100,109,110,101;,
 4;101,110,111,102;,
 4;102,111,112,103;,
 3;113,114,115;,
 3;116,114,113;,
 3;117,114,116;,
 3;118,114,117;,
 3;119,114,118;,
 3;120,114,119;,
 3;121,114,120;,
 3;115,114,121;,
 3;122,123,124;,
 3;124,123,125;,
 3;125,123,126;,
 4;126,123,127,128;,
 3;123,129,127;,
 3;129,130,127;,
 3;130,128,127;,
 3;131,130,132;,
 3;130,129,132;,
 3;129,123,132;,
 4;131,132,123,133;,
 3;133,123,122;;
 
 MeshMaterialList {
  1;
  108;
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
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neet3.jpg";
   }
  }
 }
 MeshNormals {
  92;
  0.510540;-0.603612;0.612374;,
  0.011579;-0.598241;0.801232;,
  -0.512644;-0.596480;0.617582;,
  -0.833453;-0.550616;0.046682;,
  -0.589754;-0.475101;-0.653046;,
  -0.076079;-0.434784;-0.897316;,
  0.675297;-0.379933;-0.632158;,
  0.821693;-0.568940;0.033571;,
  0.675716;-0.147741;0.722205;,
  0.004666;-0.144186;0.989540;,
  -0.677684;-0.126249;0.724434;,
  -0.995237;-0.095071;0.021540;,
  -0.711797;-0.073212;-0.698559;,
  -0.003042;0.014946;-0.999884;,
  0.704168;-0.094260;-0.703748;,
  0.992396;-0.122088;0.015617;,
  0.707107;0.000000;0.707107;,
  -0.000002;0.000000;1.000000;,
  -0.707109;0.000000;0.707104;,
  -1.000000;0.000000;0.000001;,
  -0.707109;0.000000;-0.707105;,
  -0.000001;0.000000;-1.000000;,
  0.707106;0.000000;-0.707107;,
  1.000000;0.000000;0.000001;,
  -0.000001;0.000000;1.000000;,
  -0.707109;0.000000;0.707104;,
  -0.000001;0.000000;-1.000000;,
  -0.012168;-0.999793;0.016294;,
  0.000000;1.000000;0.000000;,
  -0.335042;-0.757863;-0.559813;,
  0.481449;-0.369246;-0.794899;,
  -0.512784;0.061839;-0.856288;,
  0.703995;0.014523;0.710057;,
  0.000001;0.020450;0.999791;,
  -0.703994;0.014523;0.710058;,
  -1.000000;0.000000;0.000001;,
  -0.703994;-0.014523;-0.710057;,
  0.000001;-0.020450;-0.999791;,
  0.703995;-0.014523;-0.710056;,
  1.000000;0.000000;0.000001;,
  0.703904;0.039365;0.709203;,
  0.000001;0.055412;0.998464;,
  -0.703903;0.039365;0.709204;,
  -1.000000;-0.000001;0.000001;,
  -0.703904;-0.039367;-0.709203;,
  0.000001;-0.055414;-0.998463;,
  0.703905;-0.039367;-0.709202;,
  1.000000;-0.000001;0.000001;,
  0.707571;0.020102;0.706357;,
  0.000001;0.006638;0.999978;,
  -0.707570;0.020100;0.706358;,
  -0.999992;0.003928;-0.000098;,
  -0.706537;-0.079872;-0.703154;,
  0.000001;-0.134331;-0.990937;,
  0.706538;-0.079870;-0.703153;,
  0.999992;0.003932;-0.000098;,
  0.743416;-0.152170;0.651288;,
  -0.012618;-0.282254;0.959257;,
  -0.746463;-0.135255;0.651536;,
  -0.998809;0.018186;-0.045274;,
  -0.738162;-0.025379;-0.674146;,
  -0.026988;-0.101461;-0.994473;,
  0.727861;-0.046747;-0.684130;,
  0.999207;0.005271;-0.039457;,
  0.661896;-0.063786;0.746877;,
  -0.033736;-0.064437;0.997351;,
  -0.791389;0.139123;0.595272;,
  -0.958443;0.283625;-0.030714;,
  -0.796910;0.315075;-0.515425;,
  -0.080524;0.450444;-0.889166;,
  0.781335;0.316972;-0.537629;,
  0.964989;0.260524;-0.030385;,
  0.000000;-0.999826;0.018654;,
  -0.036820;0.357321;0.933255;,
  -0.636538;0.698611;-0.326746;,
  0.636461;0.686168;-0.352265;,
  -0.699695;0.633489;-0.330331;,
  0.000000;-1.000000;-0.000000;,
  0.423909;-0.760671;-0.491610;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.998984;0.045078;,
  0.017965;-0.999109;0.038180;,
  -0.017965;-0.999109;0.038180;,
  -0.027090;-0.999434;0.019923;,
  -0.020461;-0.999790;-0.000886;,
  0.000000;-0.999947;-0.010336;,
  0.020462;-0.999790;-0.000886;,
  0.027090;-0.999434;0.019923;,
  -0.760191;0.649454;0.017839;,
  0.548512;0.750190;-0.369255;,
  0.752571;0.658417;0.011168;,
  0.794217;0.607008;-0.027578;;
  108;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;3,11,12,4;,
  3;12,13,4;,
  3;4,13,31;,
  3;13,5,31;,
  3;5,13,30;,
  3;30,13,6;,
  3;13,14,6;,
  4;6,14,15,7;,
  4;7,15,8,0;,
  4;8,16,17,9;,
  4;9,17,18,10;,
  4;10,18,19,11;,
  4;11,19,20,12;,
  4;12,20,21,13;,
  4;13,21,22,14;,
  4;14,22,23,15;,
  4;15,23,16,8;,
  4;16,16,24,17;,
  4;17,24,25,18;,
  4;18,25,19,19;,
  4;19,19,20,20;,
  4;20,20,26,21;,
  4;21,26,22,22;,
  4;22,22,23,23;,
  4;23,23,16,16;,
  4;16,16,24,24;,
  4;24,24,25,25;,
  4;25,25,19,19;,
  4;19,19,20,20;,
  4;20,20,26,26;,
  4;26,26,22,22;,
  4;22,22,23,23;,
  4;23,23,16,16;,
  3;1,27,0;,
  3;2,27,1;,
  3;3,27,2;,
  3;4,27,3;,
  3;27,4,29;,
  3;29,4,5;,
  3;77,77,77;,
  3;6,78,30;,
  3;30,27,5;,
  3;79,79,79;,
  3;7,27,6;,
  3;0,27,7;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  3;28,28,28;,
  4;32,40,41,33;,
  4;33,41,42,34;,
  4;34,42,43,35;,
  4;35,43,44,36;,
  4;36,44,45,37;,
  4;37,45,46,38;,
  4;38,46,47,39;,
  4;39,47,40,32;,
  4;40,48,49,41;,
  4;41,49,50,42;,
  4;42,50,51,43;,
  4;43,51,52,44;,
  4;44,52,53,45;,
  4;45,53,54,46;,
  4;46,54,55,47;,
  4;47,55,48,40;,
  4;48,56,57,49;,
  4;49,57,58,50;,
  4;50,58,59,51;,
  4;51,59,60,52;,
  4;52,60,61,53;,
  4;53,61,62,54;,
  4;54,62,63,55;,
  4;55,63,56,48;,
  4;56,64,65,57;,
  4;57,65,66,58;,
  4;58,66,67,59;,
  4;59,67,68,60;,
  4;60,68,69,61;,
  4;61,69,70,62;,
  4;62,70,71,63;,
  4;63,71,64,56;,
  3;80,72,81;,
  3;82,72,80;,
  3;83,72,82;,
  3;84,72,83;,
  3;85,72,84;,
  3;86,72,85;,
  3;87,72,86;,
  3;81,72,87;,
  3;64,73,65;,
  3;65,73,66;,
  3;66,88,67;,
  4;67,88,76,68;,
  3;88,74,76;,
  3;74,69,76;,
  3;69,68,76;,
  3;70,69,75;,
  3;69,89,75;,
  3;89,90,75;,
  4;70,75,90,71;,
  3;71,90,91;;
 }
 MeshTextureCoords {
  134;
  0.199950;0.103478;,
  0.199950;0.090516;,
  0.204259;0.090516;,
  0.204259;0.103478;,
  0.208569;0.090516;,
  0.208569;0.103478;,
  0.212879;0.090516;,
  0.212879;0.103478;,
  0.217189;0.090516;,
  0.217189;0.103478;,
  0.221498;0.090516;,
  0.219416;0.103478;,
  0.221498;0.103478;,
  0.223581;0.103478;,
  0.225808;0.103478;,
  0.225808;0.090516;,
  0.230118;0.090516;,
  0.230118;0.103478;,
  0.234428;0.090516;,
  0.234428;0.103478;,
  0.199950;0.077554;,
  0.204259;0.077554;,
  0.208569;0.077554;,
  0.212879;0.077554;,
  0.217189;0.077554;,
  0.221498;0.077554;,
  0.225808;0.077554;,
  0.230118;0.077554;,
  0.234428;0.077554;,
  0.199950;0.064593;,
  0.204259;0.064593;,
  0.208569;0.064593;,
  0.212879;0.064593;,
  0.217189;0.064593;,
  0.221498;0.064593;,
  0.225808;0.064593;,
  0.230118;0.064593;,
  0.234428;0.064593;,
  0.199950;0.051631;,
  0.204259;0.051631;,
  0.208569;0.051631;,
  0.212879;0.051631;,
  0.217189;0.051631;,
  0.221498;0.051631;,
  0.225808;0.051631;,
  0.230118;0.051631;,
  0.234428;0.051631;,
  0.217189;0.146575;,
  0.217189;0.125888;,
  0.232426;0.140264;,
  0.201951;0.140264;,
  0.195640;0.125026;,
  0.201951;0.109789;,
  0.217189;0.114398;,
  0.217189;0.103478;,
  0.209825;0.106528;,
  0.232426;0.109789;,
  0.224552;0.106528;,
  0.238737;0.125026;,
  0.232426;0.014975;,
  0.217189;0.031074;,
  0.217189;0.008663;,
  0.201951;0.014975;,
  0.195640;0.030212;,
  0.201951;0.045449;,
  0.217189;0.051761;,
  0.232426;0.045449;,
  0.238737;0.030212;,
  0.221182;0.585260;,
  0.221182;0.565330;,
  0.227809;0.565330;,
  0.227809;0.585260;,
  0.234436;0.565330;,
  0.234436;0.585260;,
  0.241063;0.565330;,
  0.241063;0.585260;,
  0.247690;0.565330;,
  0.247690;0.585260;,
  0.254317;0.565330;,
  0.254317;0.585260;,
  0.260944;0.565330;,
  0.260944;0.585260;,
  0.267570;0.565330;,
  0.267570;0.585260;,
  0.274197;0.565330;,
  0.274197;0.585260;,
  0.221182;0.545399;,
  0.227809;0.545399;,
  0.234436;0.545399;,
  0.241063;0.545399;,
  0.247690;0.545399;,
  0.254317;0.545399;,
  0.260944;0.545399;,
  0.267570;0.545399;,
  0.274197;0.545399;,
  0.221182;0.525468;,
  0.227809;0.525468;,
  0.234436;0.525468;,
  0.241063;0.525468;,
  0.247690;0.525468;,
  0.254317;0.525468;,
  0.260944;0.525468;,
  0.267570;0.525468;,
  0.274197;0.525468;,
  0.221182;0.505538;,
  0.227809;0.505538;,
  0.234436;0.505538;,
  0.241063;0.505538;,
  0.247690;0.505538;,
  0.254317;0.505538;,
  0.260944;0.505538;,
  0.267570;0.505538;,
  0.274197;0.505538;,
  0.247690;0.651529;,
  0.247690;0.619720;,
  0.271119;0.641824;,
  0.224260;0.641824;,
  0.214555;0.618395;,
  0.224260;0.594965;,
  0.247690;0.585260;,
  0.271119;0.594965;,
  0.280824;0.618395;,
  0.271119;0.449173;,
  0.247690;0.473928;,
  0.247690;0.439468;,
  0.224260;0.449173;,
  0.214555;0.472602;,
  0.228536;0.491998;,
  0.224260;0.496032;,
  0.247690;0.496617;,
  0.247690;0.505737;,
  0.271119;0.496032;,
  0.267297;0.492426;,
  0.280824;0.472602;;
 }
}
