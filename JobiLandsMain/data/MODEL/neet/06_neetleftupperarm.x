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
 72;
 -20.78842;-1.46579;1.90763;,
 -14.78238;-2.57619;2.99863;,
 -15.03702;0.51345;3.96121;,
 -20.74675;0.76467;2.81678;,
 -14.65858;4.05775;3.09760;,
 -20.70471;3.01938;1.97454;,
 -14.63784;5.17669;-0.01332;,
 -20.69134;3.65325;-0.12891;,
 -14.65726;4.14433;-3.15633;,
 -20.70383;3.07796;-2.25369;,
 -14.71890;0.84533;-4.50105;,
 -20.74549;0.84749;-3.16285;,
 -14.78107;-2.48957;-3.25530;,
 -20.78752;-1.40724;-2.32061;,
 -14.80178;-3.60851;-0.14438;,
 -20.80154;-2.16372;-0.21734;,
 -14.78238;-2.57619;2.99863;,
 -20.78842;-1.46579;1.90763;,
 -11.66292;-2.66453;3.29789;,
 -11.60530;0.41927;4.65622;,
 -11.21687;4.08087;3.68892;,
 -11.18965;5.54868;0.21100;,
 -11.21540;4.17778;-3.30928;,
 -11.27906;0.77126;-4.80977;,
 -11.34331;-2.67545;-3.41152;,
 -11.37053;-4.14325;0.06640;,
 -11.66292;-2.66453;3.29789;,
 -11.66292;-2.66453;3.29789;,
 -5.96963;-2.71843;3.08922;,
 -5.90998;0.47410;4.49544;,
 -11.60530;0.41927;4.65622;,
 -5.84976;3.70424;3.18504;,
 -11.21687;4.08087;3.68892;,
 -5.82425;5.07982;-0.07436;,
 -11.18965;5.54868;0.21100;,
 -5.84839;3.79507;-3.37346;,
 -11.21540;4.17778;-3.30928;,
 -5.90804;0.60254;-4.77969;,
 -11.27906;0.77126;-4.80977;,
 -5.96826;-2.62760;-3.46928;,
 -11.34331;-2.67545;-3.41152;,
 -5.99377;-4.00318;-0.20988;,
 -11.37053;-4.14325;0.06640;,
 -5.96963;-2.71843;3.08922;,
 -11.66292;-2.66453;3.29789;,
 -1.24700;-2.24949;1.99336;,
 -1.20545;-0.02591;2.97279;,
 -1.16352;2.22389;2.06010;,
 -1.14573;3.18196;-0.21006;,
 -1.16256;2.28714;-2.50787;,
 -1.20409;0.06357;-3.48730;,
 -1.24605;-2.18624;-2.57461;,
 -1.26383;-3.14431;-0.30445;,
 -1.24700;-2.24949;1.99336;,
 -20.74675;0.76467;2.81678;,
 -21.92509;0.80697;-0.17383;,
 -20.78842;-1.46579;1.90763;,
 -20.70471;3.01938;1.97454;,
 -20.69134;3.65325;-0.12891;,
 -20.70383;3.07796;-2.25369;,
 -20.74549;0.84749;-3.16285;,
 -20.78752;-1.40724;-2.32061;,
 -20.80154;-2.16372;-0.21734;,
 -1.24700;-2.24949;1.99336;,
 0.00000;0.00000;-0.25670;,
 -1.20545;-0.02591;2.97279;,
 -1.16352;2.22389;2.06010;,
 -1.14573;3.18196;-0.21006;,
 -1.16256;2.28714;-2.50787;,
 -1.20409;0.06357;-3.48730;,
 -1.24605;-2.18624;-2.57461;,
 -1.26383;-3.14431;-0.30445;;
 
 48;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;1,18,19,2;,
 4;2,19,20,4;,
 4;4,20,21,6;,
 4;6,21,22,8;,
 4;8,22,23,10;,
 4;10,23,24,12;,
 4;12,24,25,14;,
 4;14,25,26,16;,
 4;27,28,29,30;,
 4;30,29,31,32;,
 4;32,31,33,34;,
 4;34,33,35,36;,
 4;36,35,37,38;,
 4;38,37,39,40;,
 4;40,39,41,42;,
 4;42,41,43,44;,
 4;28,45,46,29;,
 4;29,46,47,31;,
 4;31,47,48,33;,
 4;33,48,49,35;,
 4;35,49,50,37;,
 4;37,50,51,39;,
 4;39,51,52,41;,
 4;41,52,53,43;,
 3;54,55,56;,
 3;57,55,54;,
 3;58,55,57;,
 3;59,55,58;,
 3;60,55,59;,
 3;61,55,60;,
 3;62,55,61;,
 3;56,55,62;,
 3;63,64,65;,
 3;65,64,66;,
 3;66,64,67;,
 3;67,64,68;,
 3;68,64,69;,
 3;69,64,70;,
 3;70,64,71;,
 3;71,64,63;;
 
 MeshMaterialList {
  6;
  48;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neet.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neett.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\neett.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV_Grid.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\UV_Grid.png";
   }
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  42;
  -0.684130;-0.521496;0.509911;,
  -0.657290;-0.003882;0.753627;,
  -0.674756;0.530427;0.513178;,
  -0.650311;0.759599;0.010288;,
  -0.669800;0.557201;-0.490810;,
  -0.660816;0.023021;-0.750195;,
  -0.679047;-0.515819;-0.522327;,
  -0.670355;-0.741964;-0.010670;,
  -0.192878;-0.698585;0.689041;,
  -0.192080;-0.034290;0.980780;,
  -0.194651;0.674267;0.712373;,
  -0.157056;0.987501;0.013207;,
  -0.144778;0.726566;-0.671670;,
  -0.145498;0.017144;-0.989210;,
  -0.161767;-0.698201;-0.697386;,
  -0.173551;-0.984795;-0.007657;,
  -0.056864;-0.708036;0.703883;,
  -0.060006;-0.049168;0.996986;,
  -0.028814;0.674118;0.738061;,
  0.004036;0.999903;0.013356;,
  -0.007941;0.724577;-0.689148;,
  -0.033571;0.015396;-0.999318;,
  -0.046581;-0.703242;-0.709423;,
  -0.048400;-0.998828;-0.000381;,
  0.127985;-0.713450;0.688918;,
  0.180297;-0.036337;0.982941;,
  0.232988;0.668182;0.706576;,
  0.235217;0.971858;0.012866;,
  0.193473;0.705749;-0.681533;,
  0.137298;0.010296;-0.990476;,
  0.098109;-0.700256;-0.707118;,
  0.097684;-0.995193;-0.006995;,
  0.666295;-0.545990;0.507884;,
  0.701548;-0.025710;0.712158;,
  0.725665;0.475862;0.496957;,
  0.729839;0.683558;0.009144;,
  0.716291;0.496761;-0.490058;,
  0.687682;-0.003939;-0.726002;,
  0.656011;-0.537098;-0.530259;,
  0.645313;-0.763841;-0.010864;,
  -0.999775;0.021234;0.000069;,
  0.999830;-0.018441;-0.000013;;
  48;
  4;0,8,9,1;,
  4;1,9,10,2;,
  4;2,10,11,3;,
  4;3,11,12,4;,
  4;4,12,13,5;,
  4;5,13,14,6;,
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
  4;16,24,25,17;,
  4;17,25,26,18;,
  4;18,26,27,19;,
  4;19,27,28,20;,
  4;20,28,29,21;,
  4;21,29,30,22;,
  4;22,30,31,23;,
  4;23,31,24,16;,
  4;24,32,33,25;,
  4;25,33,34,26;,
  4;26,34,35,27;,
  4;27,35,36,28;,
  4;28,36,37,29;,
  4;29,37,38,30;,
  4;30,38,39,31;,
  4;31,39,32,24;,
  3;1,40,0;,
  3;2,40,1;,
  3;3,40,2;,
  3;4,40,3;,
  3;5,40,4;,
  3;6,40,5;,
  3;7,40,6;,
  3;0,40,7;,
  3;32,41,33;,
  3;33,41,34;,
  3;34,41,35;,
  3;35,41,36;,
  3;36,41,37;,
  3;37,41,38;,
  3;38,41,39;,
  3;39,41,32;;
 }
 MeshTextureCoords {
  72;
  0.158475;0.260259;,
  0.158475;0.237151;,
  0.166158;0.237151;,
  0.166158;0.260259;,
  0.173841;0.237151;,
  0.173841;0.260259;,
  0.181525;0.237151;,
  0.181525;0.260259;,
  0.189208;0.237151;,
  0.189208;0.260259;,
  0.196892;0.237151;,
  0.196892;0.260259;,
  0.204575;0.237151;,
  0.204575;0.260259;,
  0.212259;0.237151;,
  0.212259;0.260259;,
  0.219942;0.237151;,
  0.219942;0.260259;,
  0.298740;0.552824;,
  0.302828;0.552824;,
  0.306915;0.552824;,
  0.311002;0.552824;,
  0.315090;0.552824;,
  0.319177;0.552824;,
  0.323264;0.552824;,
  0.327352;0.552824;,
  0.331439;0.552824;,
  0.477399;0.961667;,
  0.477399;0.941948;,
  0.483956;0.941948;,
  0.483956;0.961667;,
  0.490513;0.941948;,
  0.490513;0.961667;,
  0.497070;0.941948;,
  0.497070;0.961667;,
  0.503627;0.941948;,
  0.503627;0.961667;,
  0.510183;0.941948;,
  0.510183;0.961667;,
  0.516740;0.941948;,
  0.516740;0.961667;,
  0.523297;0.941948;,
  0.523297;0.961667;,
  0.529854;0.941948;,
  0.529854;0.961667;,
  0.477399;0.922228;,
  0.483956;0.922228;,
  0.490513;0.922228;,
  0.497070;0.922228;,
  0.503627;0.922228;,
  0.510183;0.922228;,
  0.516740;0.922228;,
  0.523297;0.922228;,
  0.529854;0.922228;,
  0.189208;0.337093;,
  0.189208;0.300213;,
  0.216373;0.325841;,
  0.162043;0.325841;,
  0.150791;0.298676;,
  0.162043;0.271511;,
  0.189208;0.260259;,
  0.216373;0.271511;,
  0.227625;0.298676;,
  0.526808;0.866459;,
  0.503627;0.890952;,
  0.503627;0.856857;,
  0.480445;0.866459;,
  0.470843;0.889641;,
  0.480445;0.912823;,
  0.503627;0.922425;,
  0.526808;0.912823;,
  0.536410;0.889641;;
 }
}
