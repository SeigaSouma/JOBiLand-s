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
 186;
 2.65216;2.51869;-0.46826;,
 3.19889;2.51746;-0.83182;,
 2.93147;2.29467;-1.75860;,
 1.89027;2.39476;-1.06569;,
 3.85483;2.50890;-0.80466;,
 6.33256;2.16521;-2.36514;,
 4.36948;2.49633;-0.39715;,
 6.56103;2.12100;-1.02783;,
 4.54620;2.48448;0.23507;,
 6.51905;1.88384;0.30824;,
 4.31759;2.47797;0.85049;,
 6.44965;2.08160;1.64958;,
 3.77085;2.47920;1.21406;,
 6.11090;2.08671;2.96402;,
 3.11490;2.48772;1.18690;,
 2.77174;2.23809;2.08124;,
 2.60026;2.50028;0.77938;,
 1.79154;2.35981;1.30746;,
 2.42352;2.51215;0.14717;,
 1.45533;2.38229;0.10492;,
 2.65216;2.51869;-0.46826;,
 1.89027;2.39476;-1.06569;,
 2.71086;2.02564;-2.49586;,
 1.27956;2.02890;-1.54403;,
 6.99160;1.88417;-2.31987;,
 7.17980;1.98961;-1.02342;,
 7.09495;1.79409;0.33079;,
 7.06683;1.94965;1.69253;,
 6.77175;1.80636;2.96522;,
 2.49098;1.94783;2.78923;,
 1.14367;1.98078;1.72234;,
 0.68095;2.01174;0.06718;,
 1.27956;2.02890;-1.54403;,
 2.56754;1.07630;-2.98066;,
 0.88494;1.08007;-1.86171;,
 7.47240;0.86252;-2.36990;,
 7.55660;0.93767;-1.06612;,
 7.54560;0.95107;0.33712;,
 7.44005;0.89642;1.73543;,
 7.24797;0.78306;3.02494;,
 2.30910;0.98480;3.23234;,
 0.72521;1.02353;1.97813;,
 0.18124;1.05992;0.03238;,
 0.88494;1.08007;-1.86171;,
 2.50911;0.01720;-3.15887;,
 0.73990;0.02118;-1.98234;,
 7.90452;-0.10947;-2.39870;,
 7.97563;-0.03666;-1.06005;,
 8.00297;-0.00871;0.34201;,
 7.85934;-0.07784;1.73554;,
 7.67740;-0.18988;3.06094;,
 4.84570;-0.13218;3.22382;,
 2.23735;-0.07902;3.37386;,
 0.57195;-0.03831;2.05511;,
 0.00000;0.00000;0.00923;,
 0.73990;0.02118;-1.98234;,
 2.54124;-1.04797;-3.01304;,
 0.85864;-1.04419;-1.89409;,
 7.02608;-1.59326;-2.48920;,
 7.04262;-1.59887;-1.10760;,
 7.15601;-1.66254;0.27154;,
 6.92443;-1.65075;1.63724;,
 6.78920;-1.69723;3.01154;,
 4.89964;-0.14668;3.16064;,
 0.69891;-1.10074;1.94575;,
 2.28280;-1.13948;3.19995;,
 0.15493;-1.06435;-0.00000;,
 0.85864;-1.04419;-1.89409;,
 2.64952;-1.17886;-2.54576;,
 1.22952;-2.01171;-1.60562;,
 6.06796;-1.10726;-2.48342;,
 5.68588;-1.90288;-1.10543;,
 5.74233;-2.00231;0.20424;,
 5.57338;-1.95225;1.50702;,
 5.33876;-1.88249;2.90756;,
 4.91548;-0.16805;2.89713;,
 1.09364;-2.05981;1.66075;,
 2.44100;-2.09275;2.72764;,
 0.63092;-2.02885;0.00559;,
 1.22952;-2.01171;-1.60562;,
 2.83536;-1.25527;-1.81525;,
 1.81626;-2.78668;-1.14515;,
 6.27892;-1.20827;-2.47899;,
 5.23149;-1.13104;-1.12634;,
 5.19626;-1.20268;0.19562;,
 5.11764;-1.18101;1.51748;,
 4.93943;-0.83572;2.92320;,
 4.91339;-0.24799;2.67184;,
 1.71752;-2.82165;1.22801;,
 2.69641;-2.84559;2.00315;,
 1.38133;-2.79915;0.02547;,
 1.81626;-2.78668;-1.14515;,
 5.59561;-0.81976;-0.76455;,
 5.04890;-0.81855;-0.40098;,
 6.25156;-0.82832;-0.73738;,
 6.76619;-0.84089;-0.32987;,
 6.94294;-0.85273;0.30235;,
 6.71428;-0.85927;0.91777;,
 6.16759;-0.85804;1.28134;,
 5.51162;-0.84951;1.25418;,
 4.99701;-0.83693;0.84666;,
 4.82025;-0.82510;0.21445;,
 5.04890;-0.81855;-0.40098;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 3.48869;2.53970;0.19191;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 5.89291;-0.65789;0.26153;,
 2.29390;0.03180;-2.45476;,
 2.10320;-0.53139;-2.52321;,
 2.92507;-0.83770;-2.07658;,
 3.12768;-0.26595;-2.22785;,
 1.95249;-0.73219;-2.72382;,
 3.49758;-1.63603;-2.70101;,
 1.87699;-0.96305;-2.81697;,
 3.44184;-1.85265;-3.15811;,
 1.95994;-0.72682;-2.86121;,
 3.54576;-1.60139;-3.58828;,
 2.11914;-0.51993;-2.81669;,
 3.74828;-1.02988;-3.73955;,
 2.30135;0.03715;-2.59215;,
 3.93095;-0.47248;-3.52330;,
 2.37562;0.26701;-2.47718;,
 3.98664;-0.25598;-3.06620;,
 2.29390;0.03180;-2.45476;,
 3.12768;-0.26595;-2.22785;,
 4.38687;-1.32166;-2.45586;,
 4.58949;-0.74993;-2.60713;,
 4.20429;-1.87881;-2.67211;,
 4.14853;-2.09547;-3.12922;,
 4.25248;-1.84420;-3.55939;,
 4.45497;-1.27269;-3.71067;,
 4.63766;-0.71529;-3.49441;,
 4.69335;-0.49879;-3.03731;,
 4.58949;-0.74993;-2.60713;,
 7.10421;-2.32518;-1.51159;,
 7.08988;-1.47445;-1.54707;,
 7.05717;-2.92834;-1.73860;,
 7.32902;-3.25492;-2.22138;,
 7.76032;-3.11422;-2.67732;,
 8.09860;-2.58804;-2.83926;,
 8.14550;-1.98537;-2.61234;,
 7.87370;-1.65858;-2.12947;,
 7.08988;-1.47445;-1.54707;,
 7.06098;-2.39663;0.30403;,
 7.38961;-1.87785;0.31822;,
 7.00442;-3.00669;0.25324;,
 7.25322;-3.34988;0.19564;,
 7.66140;-3.22577;0.16494;,
 7.99014;-2.70650;0.17916;,
 8.04657;-2.09691;0.22992;,
 7.33024;-1.61111;0.70745;,
 7.38961;-1.87785;0.31822;,
 2.10320;-0.53139;-2.52321;,
 2.29390;0.03180;-2.45476;,
 2.11753;-0.52111;-2.78666;,
 1.95249;-0.73219;-2.72382;,
 1.87699;-0.96305;-2.81697;,
 1.95994;-0.72682;-2.86121;,
 2.11914;-0.51993;-2.81669;,
 2.30135;0.03715;-2.59215;,
 2.37562;0.26701;-2.47718;,
 7.38961;-1.87785;0.31822;,
 7.06098;-2.39663;0.30403;,
 7.50288;-2.56793;0.66029;,
 7.00442;-3.00669;0.25324;,
 7.25322;-3.34988;0.19564;,
 7.66140;-3.22577;0.16494;,
 7.99014;-2.70650;0.17916;,
 8.04657;-2.09691;0.22992;,
 7.33024;-1.61111;0.70745;;
 
 155;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 4;16,18,19,17;,
 4;18,20,21,19;,
 4;3,2,22,23;,
 4;2,5,24,22;,
 4;5,7,25,24;,
 4;7,9,26,25;,
 4;9,11,27,26;,
 4;11,13,28,27;,
 4;13,15,29,28;,
 4;15,17,30,29;,
 4;17,19,31,30;,
 4;19,21,32,31;,
 4;23,22,33,34;,
 4;22,24,35,33;,
 4;24,25,36,35;,
 4;25,26,37,36;,
 4;26,27,38,37;,
 4;27,28,39,38;,
 4;28,29,40,39;,
 4;29,30,41,40;,
 4;30,31,42,41;,
 4;31,32,43,42;,
 4;34,33,44,45;,
 4;33,35,46,44;,
 4;35,36,47,46;,
 4;36,37,48,47;,
 4;37,38,49,48;,
 4;38,39,50,49;,
 3;50,39,51;,
 3;39,40,51;,
 3;40,52,51;,
 4;40,41,53,52;,
 4;41,42,54,53;,
 4;42,43,55,54;,
 4;45,44,56,57;,
 4;44,46,58,56;,
 4;46,47,59,58;,
 4;47,48,60,59;,
 4;48,49,61,60;,
 4;49,50,62,61;,
 4;50,51,63,62;,
 4;52,53,64,65;,
 4;53,54,66,64;,
 4;54,55,67,66;,
 4;57,56,68,69;,
 4;56,58,70,68;,
 4;58,59,71,70;,
 4;59,60,72,71;,
 4;60,61,73,72;,
 4;61,62,74,73;,
 4;62,63,75,74;,
 4;65,64,76,77;,
 4;64,66,78,76;,
 4;66,67,79,78;,
 4;69,68,80,81;,
 4;68,70,82,80;,
 4;70,71,83,82;,
 4;71,72,84,83;,
 4;72,73,85,84;,
 4;73,74,86,85;,
 4;74,75,87,86;,
 4;77,76,88,89;,
 4;76,78,90,88;,
 4;78,79,91,90;,
 4;81,80,92,93;,
 4;80,82,94,92;,
 4;82,83,95,94;,
 4;83,84,96,95;,
 4;84,85,97,96;,
 4;85,86,98,97;,
 3;98,86,99;,
 3;86,87,99;,
 3;87,89,99;,
 4;89,88,100,99;,
 4;88,90,101,100;,
 4;90,91,102,101;,
 3;1,0,103;,
 3;4,1,104;,
 3;6,4,105;,
 3;8,6,106;,
 3;10,8,107;,
 3;12,10,108;,
 3;14,12,109;,
 3;16,14,110;,
 3;18,16,111;,
 3;20,18,112;,
 3;93,92,113;,
 3;92,94,114;,
 3;94,95,115;,
 3;95,96,116;,
 3;96,97,117;,
 3;97,98,118;,
 3;98,99,119;,
 3;99,100,120;,
 3;100,101,121;,
 3;101,102,122;,
 4;63,51,52,65;,
 4;75,63,65,77;,
 4;87,75,77,89;,
 4;123,124,125,126;,
 4;124,127,128,125;,
 4;127,129,130,128;,
 4;129,131,132,130;,
 4;131,133,134,132;,
 4;133,135,136,134;,
 4;135,137,138,136;,
 4;137,139,140,138;,
 4;126,125,141,142;,
 4;125,128,143,141;,
 4;128,130,144,143;,
 4;130,132,145,144;,
 4;132,134,146,145;,
 4;134,136,147,146;,
 4;136,138,148,147;,
 4;138,140,149,148;,
 4;142,141,150,151;,
 4;141,143,152,150;,
 4;143,144,153,152;,
 4;144,145,154,153;,
 4;145,146,155,154;,
 4;146,147,156,155;,
 4;147,148,157,156;,
 4;148,149,158,157;,
 4;151,150,159,160;,
 4;150,152,161,159;,
 4;152,153,162,161;,
 4;153,154,163,162;,
 4;154,155,164,163;,
 4;155,156,165,164;,
 4;156,157,166,165;,
 4;157,158,167,166;,
 3;168,169,170;,
 3;171,168,170;,
 3;172,171,170;,
 3;173,172,170;,
 3;174,173,170;,
 3;175,174,170;,
 3;176,175,170;,
 3;169,176,170;,
 3;177,178,179;,
 3;178,180,179;,
 3;180,181,179;,
 3;181,182,179;,
 3;182,183,179;,
 3;183,184,179;,
 3;184,185,179;,
 3;185,177,179;;
 
 MeshMaterialList {
  1;
  155;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\MORS\\enemylelftarm.jpg";
   }
  }
 }
 MeshNormals {
  189;
  -0.060959;0.996284;-0.060844;,
  -0.015383;0.993187;-0.115514;,
  0.048063;0.995810;-0.077800;,
  0.110726;0.993832;0.006125;,
  0.138734;0.990121;0.020342;,
  0.109640;0.993447;0.032293;,
  0.040198;0.993033;0.110766;,
  -0.026180;0.989377;0.142994;,
  -0.067036;0.994136;0.084856;,
  -0.079951;0.996734;0.011374;,
  -0.208666;0.960342;-0.184938;,
  -0.065908;0.902684;-0.425227;,
  0.122345;0.939635;-0.319560;,
  0.208552;0.977484;0.032124;,
  0.204010;0.978701;0.022888;,
  0.209292;0.977752;0.014048;,
  0.094225;0.929653;0.356184;,
  -0.102132;0.889827;0.444722;,
  -0.224496;0.954736;0.195141;,
  -0.285172;0.958474;0.002262;,
  -0.528289;0.737020;-0.421559;,
  -0.159327;0.597493;-0.785886;,
  0.154196;0.682186;-0.714735;,
  0.648286;0.761371;0.006331;,
  0.622103;0.782042;0.037387;,
  0.645640;0.760436;0.069905;,
  0.355646;0.652978;0.668682;,
  -0.224784;0.574241;0.787223;,
  -0.562385;0.724939;0.397725;,
  -0.689677;0.723893;-0.018027;,
  -0.741463;0.327843;-0.585451;,
  -0.217285;0.275338;-0.936470;,
  0.113378;0.204519;-0.972274;,
  0.915308;0.400866;-0.038967;,
  0.911975;0.407885;0.043943;,
  0.908423;0.398421;0.126605;,
  0.902628;0.387902;0.186531;,
  -0.224002;0.225294;0.948191;,
  -0.787918;0.311392;0.531244;,
  -0.946114;0.321977;-0.034615;,
  -0.783829;0.019169;-0.620681;,
  -0.226172;0.023247;-0.973810;,
  0.127785;0.023997;-0.991512;,
  0.997788;-0.050590;-0.043121;,
  0.998352;-0.040471;0.040686;,
  0.990897;-0.052763;0.123853;,
  0.989235;-0.064020;0.131587;,
  -0.273376;-0.122728;0.954046;,
  -0.832683;0.001868;0.553746;,
  -0.999059;0.013004;-0.041367;,
  -0.716063;-0.393257;-0.576717;,
  -0.262010;-0.353833;-0.897860;,
  0.119266;-0.077097;-0.989864;,
  0.705613;-0.706807;-0.050338;,
  0.600983;-0.799187;0.010940;,
  0.580826;-0.810784;0.072600;,
  0.565409;-0.822979;0.054938;,
  -0.180930;-0.546758;0.817508;,
  -0.795582;-0.307843;0.521807;,
  -0.953781;-0.297255;-0.044053;,
  -0.438540;-0.740000;-0.509983;,
  -0.029449;-0.788367;-0.614500;,
  -0.115267;-0.983969;-0.136085;,
  0.044273;-0.985799;-0.161988;,
  0.224627;-0.974406;-0.008743;,
  0.196012;-0.979670;0.042725;,
  0.170504;-0.984925;0.029161;,
  -0.045642;-0.740640;0.670351;,
  -0.680381;-0.587087;0.438647;,
  -0.814868;-0.578093;-0.042413;,
  0.390027;-0.887753;-0.244486;,
  0.150458;-0.931280;-0.331784;,
  -0.036074;-0.979468;-0.198344;,
  -0.400853;0.915432;-0.036069;,
  -0.842933;-0.536011;-0.046429;,
  -0.871974;-0.484580;-0.069593;,
  -0.961283;-0.234126;0.145325;,
  0.035992;-0.788884;0.613487;,
  -0.597558;-0.705638;0.380788;,
  -0.715198;-0.697775;-0.040031;,
  0.274118;-0.961437;0.022322;,
  0.110600;-0.991607;0.066950;,
  -0.040971;-0.984297;0.171700;,
  -0.276455;0.952686;0.126337;,
  -0.202731;0.979201;0.008047;,
  -0.281610;0.931448;-0.230436;,
  -0.350887;0.843764;-0.406128;,
  0.235598;-0.968715;-0.078000;,
  0.297139;-0.926383;-0.231352;,
  0.330426;-0.943832;-0.000257;,
  0.012452;0.999806;0.015241;,
  -0.013160;-0.999797;-0.015275;,
  0.083677;-0.195246;0.977178;,
  0.023924;-0.660323;0.750601;,
  -0.584755;-0.275317;0.763061;,
  -0.642116;0.006899;0.766577;,
  -0.098433;0.380579;0.919495;,
  -0.413338;-0.266118;0.870823;,
  -0.369081;-0.628804;0.684387;,
  -0.304964;-0.608082;0.732962;,
  -0.457706;-0.115905;-0.881517;,
  -0.358098;0.303612;-0.882942;,
  -0.205267;0.597974;-0.774786;,
  -0.064823;0.729124;-0.681305;,
  0.117378;0.468886;0.875425;,
  -0.148863;-0.228958;0.961987;,
  -0.288279;-0.663060;0.690830;,
  -0.312155;-0.739689;0.596170;,
  -0.323779;-0.349909;-0.879051;,
  -0.160713;0.179309;-0.970577;,
  0.014472;0.627507;-0.778477;,
  0.125002;0.808622;-0.574896;,
  0.141863;0.609619;0.779897;,
  -0.129289;-0.107683;0.985743;,
  -0.321126;-0.644019;0.694347;,
  -0.329040;-0.942615;-0.056660;,
  -0.097906;-0.601195;-0.793082;,
  0.142605;0.008355;-0.989744;,
  0.320193;0.613067;-0.722237;,
  0.333092;0.942840;-0.010161;,
  -0.303411;0.188236;0.934082;,
  -0.357673;-0.035791;0.933161;,
  -0.616702;-0.634792;0.465529;,
  -0.327588;-0.943357;-0.052569;,
  0.331075;-0.829188;-0.450374;,
  0.230551;-0.022519;-0.972800;,
  0.551714;0.691436;-0.466399;,
  0.466400;0.884566;0.003784;,
  -0.896920;0.369779;0.242483;,
  -0.869396;0.186765;0.457459;,
  -0.839245;-0.271595;0.471067;,
  -0.807098;-0.588586;-0.046468;,
  0.562798;-0.704905;0.431703;,
  0.857450;-0.315733;0.406316;,
  0.863918;0.159656;0.477657;,
  0.741854;0.437677;0.508027;,
  -0.899326;0.437004;-0.015522;,
  -0.166050;-0.139553;0.976193;,
  0.915215;0.383842;-0.122664;,
  0.914522;0.392123;-0.099445;,
  0.038581;0.138308;0.989638;,
  0.996754;-0.062063;-0.051287;,
  0.040928;-0.314463;0.948387;,
  0.859710;-0.510560;-0.015060;,
  -0.238761;-0.610824;0.754909;,
  -0.084796;-0.880677;0.466065;,
  -0.120508;-0.985536;0.119148;,
  -0.140237;-0.959691;-0.243571;,
  -0.767445;-0.364622;0.527332;,
  -0.595637;0.720793;-0.354505;,
  -0.495886;0.864081;-0.086383;,
  -0.843562;-0.536256;-0.028852;,
  -0.687724;-0.676769;-0.262715;,
  -0.345370;0.918911;0.190583;,
  -0.043012;-0.992161;-0.117333;,
  -0.013158;-0.999906;0.003751;,
  -0.931431;-0.177210;-0.317858;,
  0.552573;-0.656918;0.512954;,
  0.583070;-0.629760;0.513256;,
  0.395878;-0.754663;-0.523225;,
  0.455996;-0.819957;-0.346032;,
  0.497505;-0.867427;0.007724;,
  -0.153178;-0.985066;0.078626;,
  -0.181570;-0.983131;-0.022038;,
  -0.144925;-0.982139;-0.119995;,
  0.434191;-0.722762;0.537674;,
  -0.705198;0.033527;-0.708217;,
  -0.394892;-0.618523;-0.679330;,
  0.124941;0.559589;0.819298;,
  0.228678;0.712796;0.663045;,
  -0.203886;0.966003;0.158959;,
  -0.958058;0.274495;0.082325;,
  -0.982566;0.183378;0.030594;,
  -0.995697;0.090138;-0.021492;,
  0.290005;-0.851117;0.437604;,
  0.945963;-0.323609;0.020757;,
  0.995244;-0.092148;0.031585;,
  -0.549175;0.835023;-0.033809;,
  -0.879552;0.474898;-0.029311;,
  -0.945033;0.324678;-0.038693;,
  -0.796583;0.544888;-0.261825;,
  -0.807917;0.515354;0.285797;,
  -0.742802;0.365837;0.560721;,
  -0.879675;0.474720;-0.028514;,
  -0.945117;0.324497;-0.038157;,
  -0.945063;0.324611;-0.038518;,
  -0.347346;-0.394588;0.850676;,
  0.640027;-0.259941;0.723046;,
  -0.983936;-0.176083;-0.029411;;
  155;
  4;0,1,11,10;,
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;6,7,17,16;,
  4;7,8,18,17;,
  4;8,9,19,18;,
  4;9,0,10,19;,
  4;10,11,21,20;,
  4;11,12,22,21;,
  4;12,13,23,22;,
  4;13,14,24,23;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  4;17,18,28,27;,
  4;18,19,29,28;,
  4;19,10,20,29;,
  4;20,21,31,30;,
  4;21,22,32,31;,
  4;138,23,33,139;,
  4;23,24,34,33;,
  4;24,25,35,34;,
  4;25,26,36,35;,
  4;26,27,37,140;,
  4;27,28,38,37;,
  4;28,29,39,38;,
  4;29,20,30,39;,
  4;30,31,41,40;,
  4;31,32,42,41;,
  4;139,33,43,141;,
  4;33,34,44,43;,
  4;34,35,45,44;,
  4;35,36,46,45;,
  3;142,140,92;,
  3;140,37,92;,
  3;37,47,92;,
  4;37,38,48,47;,
  4;38,39,49,48;,
  4;39,30,40,49;,
  4;40,41,51,50;,
  4;41,42,52,51;,
  4;141,43,53,143;,
  4;43,44,54,53;,
  4;44,45,55,54;,
  4;45,46,56,55;,
  4;142,92,93,144;,
  4;47,48,58,57;,
  4;48,49,59,58;,
  4;49,40,50,59;,
  4;50,51,61,60;,
  4;145,146,62,145;,
  4;146,147,63,62;,
  4;53,54,64,63;,
  4;54,55,65,64;,
  4;55,56,66,65;,
  4;144,93,94,148;,
  4;57,58,68,67;,
  4;58,59,69,68;,
  4;59,50,60,69;,
  4;60,61,71,70;,
  4;61,62,72,71;,
  4;149,149,73,150;,
  4;151,74,74,151;,
  4;74,75,75,74;,
  4;75,148,76,75;,
  4;148,94,95,76;,
  4;67,68,78,77;,
  4;68,69,79,78;,
  4;69,60,152,79;,
  4;70,71,81,80;,
  4;71,72,82,81;,
  4;150,73,83,153;,
  4;73,84,84,83;,
  4;84,85,85,84;,
  4;85,86,86,85;,
  3;154,155,87;,
  3;76,156,156;,
  3;157,158,87;,
  4;159,160,88,87;,
  4;160,161,89,88;,
  4;161,70,80,89;,
  3;1,0,90;,
  3;2,1,90;,
  3;3,2,90;,
  3;4,3,90;,
  3;5,4,90;,
  3;6,5,90;,
  3;7,6,90;,
  3;8,7,90;,
  3;9,8,90;,
  3;0,9,90;,
  3;80,81,91;,
  3;81,82,91;,
  3;82,162,91;,
  3;162,163,91;,
  3;163,164,91;,
  3;164,154,91;,
  3;154,87,91;,
  3;87,88,91;,
  3;88,89,91;,
  3;89,80,91;,
  4;93,92,47,57;,
  4;165,93,57,67;,
  4;157,165,67,77;,
  4;96,97,105,104;,
  4;97,98,106,105;,
  4;98,99,107,106;,
  4;166,100,108,167;,
  4;100,101,109,108;,
  4;101,102,110,109;,
  4;102,103,111,110;,
  4;168,96,104,169;,
  4;104,105,113,112;,
  4;105,106,114,113;,
  4;106,107,115,114;,
  4;167,108,116,115;,
  4;108,109,117,116;,
  4;109,110,118,117;,
  4;110,111,119,118;,
  4;169,104,112,119;,
  4;112,113,121,120;,
  4;113,114,122,121;,
  4;114,115,123,122;,
  4;115,116,124,123;,
  4;116,117,125,124;,
  4;117,118,126,125;,
  4;118,119,127,126;,
  4;119,112,170,127;,
  4;171,172,129,128;,
  4;172,173,130,129;,
  4;122,123,131,130;,
  4;123,124,132,174;,
  4;124,175,133,132;,
  4;175,176,134,133;,
  4;126,127,135,134;,
  4;177,170,128,177;,
  3;178,179,136;,
  3;180,178,136;,
  3;166,180,136;,
  3;181,182,136;,
  3;183,181,136;,
  3;184,183,136;,
  3;185,184,136;,
  3;179,185,136;,
  3;128,129,137;,
  3;129,130,137;,
  3;130,186,137;,
  3;174,132,137;,
  3;132,133,187;,
  3;133,134,187;,
  3;134,135,137;,
  3;188,128,188;;
 }
 MeshTextureCoords {
  186;
  0.544731;0.727846;,
  0.558024;0.727846;,
  0.558024;0.714554;,
  0.544731;0.714554;,
  0.571316;0.727846;,
  0.571316;0.714554;,
  0.584608;0.727846;,
  0.584608;0.714554;,
  0.597901;0.727846;,
  0.597901;0.714554;,
  0.611193;0.727846;,
  0.611193;0.714554;,
  0.624485;0.727846;,
  0.624485;0.714554;,
  0.637778;0.727846;,
  0.637778;0.714554;,
  0.651070;0.727846;,
  0.651070;0.714554;,
  0.664363;0.727846;,
  0.664363;0.714554;,
  0.677655;0.727846;,
  0.677655;0.714554;,
  0.558024;0.701261;,
  0.544731;0.701261;,
  0.571316;0.701261;,
  0.584608;0.701261;,
  0.597901;0.701261;,
  0.611193;0.701261;,
  0.624485;0.701261;,
  0.637778;0.701261;,
  0.651070;0.701261;,
  0.664363;0.701261;,
  0.677655;0.701261;,
  0.558024;0.687969;,
  0.544731;0.687969;,
  0.571316;0.687969;,
  0.584608;0.687969;,
  0.597901;0.687969;,
  0.611193;0.687969;,
  0.624485;0.687969;,
  0.637778;0.687969;,
  0.651070;0.687969;,
  0.664363;0.687969;,
  0.677655;0.687969;,
  0.558024;0.674677;,
  0.544731;0.674677;,
  0.571316;0.674677;,
  0.584608;0.674677;,
  0.597901;0.674677;,
  0.611193;0.674677;,
  0.624485;0.674677;,
  0.631405;0.674677;,
  0.637778;0.674677;,
  0.651070;0.674677;,
  0.664363;0.674677;,
  0.677655;0.674677;,
  0.558024;0.661384;,
  0.544731;0.661384;,
  0.571316;0.661384;,
  0.584608;0.661384;,
  0.597901;0.661384;,
  0.611193;0.661384;,
  0.624485;0.661384;,
  0.631714;0.661384;,
  0.651070;0.661384;,
  0.637778;0.661384;,
  0.664363;0.661384;,
  0.677655;0.661384;,
  0.558024;0.648092;,
  0.544731;0.648092;,
  0.571316;0.648092;,
  0.584608;0.648092;,
  0.597901;0.648092;,
  0.611193;0.648092;,
  0.624485;0.648092;,
  0.630495;0.648092;,
  0.651070;0.648092;,
  0.637778;0.648092;,
  0.664363;0.648092;,
  0.677655;0.648092;,
  0.558024;0.634800;,
  0.544731;0.634800;,
  0.571316;0.634800;,
  0.584608;0.634800;,
  0.597901;0.634800;,
  0.611193;0.634800;,
  0.624485;0.634800;,
  0.628913;0.634800;,
  0.651070;0.634800;,
  0.637778;0.634800;,
  0.664363;0.634800;,
  0.677655;0.634800;,
  0.558024;0.621507;,
  0.544731;0.621507;,
  0.571316;0.621507;,
  0.584608;0.621507;,
  0.597901;0.621507;,
  0.611193;0.621507;,
  0.624485;0.621507;,
  0.637778;0.621507;,
  0.651070;0.621507;,
  0.664363;0.621507;,
  0.677655;0.621507;,
  0.551377;0.741139;,
  0.564670;0.741139;,
  0.577962;0.741139;,
  0.591255;0.741139;,
  0.604547;0.741139;,
  0.617839;0.741139;,
  0.631132;0.741139;,
  0.644424;0.741139;,
  0.657716;0.741139;,
  0.671009;0.741139;,
  0.551377;0.608215;,
  0.564670;0.608215;,
  0.577962;0.608215;,
  0.591255;0.608215;,
  0.604547;0.608215;,
  0.617839;0.608215;,
  0.631132;0.608215;,
  0.644424;0.608215;,
  0.657716;0.608215;,
  0.671009;0.608215;,
  0.622053;0.723150;,
  0.626207;0.723150;,
  0.626207;0.710657;,
  0.622053;0.710657;,
  0.630361;0.723150;,
  0.630361;0.710657;,
  0.634514;0.723150;,
  0.634514;0.710657;,
  0.638668;0.723150;,
  0.638668;0.710657;,
  0.642822;0.723150;,
  0.642822;0.710657;,
  0.646976;0.723150;,
  0.646976;0.710657;,
  0.651130;0.723150;,
  0.651130;0.710657;,
  0.655284;0.723150;,
  0.655284;0.710657;,
  0.626207;0.698164;,
  0.622053;0.698164;,
  0.630361;0.698164;,
  0.634514;0.698164;,
  0.638668;0.698164;,
  0.642822;0.698164;,
  0.646976;0.698164;,
  0.651130;0.698164;,
  0.655284;0.698164;,
  0.626207;0.685672;,
  0.622053;0.685672;,
  0.630361;0.685672;,
  0.634514;0.685672;,
  0.638668;0.685672;,
  0.642822;0.685672;,
  0.646976;0.685672;,
  0.651130;0.685672;,
  0.655284;0.685672;,
  0.626207;0.673179;,
  0.622053;0.673179;,
  0.630361;0.673179;,
  0.634514;0.673179;,
  0.638668;0.673179;,
  0.642822;0.673179;,
  0.646976;0.673179;,
  0.651130;0.673179;,
  0.655284;0.673179;,
  0.638668;0.764689;,
  0.653354;0.758605;,
  0.638668;0.744750;,
  0.623982;0.758605;,
  0.617899;0.743919;,
  0.623982;0.729233;,
  0.638668;0.723150;,
  0.653354;0.729233;,
  0.659438;0.743919;,
  0.653354;0.637848;,
  0.638668;0.631765;,
  0.638668;0.653365;,
  0.623982;0.637848;,
  0.617899;0.652534;,
  0.623982;0.667221;,
  0.638668;0.673304;,
  0.653354;0.667221;,
  0.659438;0.652534;;
 }
}
