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
 132;
 -1.41936;-1.71486;5.25956;,
 -2.32502;-0.67323;5.27086;,
 -3.95012;-1.24958;4.44650;,
 -2.74830;-11.00157;4.93303;,
 -2.32498;0.61431;5.27678;,
 -3.57533;1.19946;4.45775;,
 -1.41925;1.65594;5.15389;,
 -2.74808;3.18077;4.45446;,
 0.04168;2.05381;5.26637;,
 0.03572;3.93756;4.43790;,
 1.50084;1.65594;5.13287;,
 2.81947;3.18077;4.41440;,
 2.40824;0.61431;5.24272;,
 3.64668;1.19946;4.40578;,
 2.40820;-0.67323;5.23681;,
 4.02126;-1.24958;4.38914;,
 1.50247;-1.71486;5.23854;,
 2.82657;-11.00157;4.89292;,
 1.56618;-6.93341;5.31486;,
 0.03911;-11.07446;5.82608;,
 0.04606;-8.48731;5.87569;,
 -1.41936;-1.71486;5.25956;,
 -1.48793;-6.93341;5.33684;,
 -2.74830;-11.00157;4.93303;,
 -3.94359;-6.10963;4.10455;,
 -5.07555;-1.70361;3.47321;,
 -4.90568;-11.10417;3.43770;,
 -4.49226;1.66720;3.48870;,
 -2.78559;4.39423;3.46945;,
 0.02869;5.43587;3.46135;,
 2.84280;4.39423;3.42896;,
 4.54957;1.66719;3.42364;,
 5.13258;-1.70361;3.39976;,
 4.98293;-10.99180;3.38644;,
 4.02130;-6.04737;4.05826;,
 0.02833;-6.29138;3.41123;,
 -4.90568;-11.10417;3.43770;,
 -3.94359;-6.10963;4.10455;,
 -5.56156;-1.99088;1.60039;,
 -5.59282;-8.01955;1.55969;,
 -5.25703;1.97174;1.61860;,
 -3.47033;5.17757;1.59853;,
 0.01520;6.40208;1.58645;,
 3.50055;5.17756;1.54837;,
 5.28735;1.97174;1.54273;,
 5.59159;-1.99088;1.52014;,
 5.64297;-7.90718;1.49874;,
 3.21841;-5.21740;1.49161;,
 0.01478;-3.84875;1.52753;,
 -3.18505;-5.23931;1.53380;,
 -5.59282;-8.01955;1.55969;,
 -5.97675;-2.08327;-0.48059;,
 -5.83992;-10.62651;-0.52309;,
 -5.43745;2.08327;-0.46145;,
 -4.03703;5.19521;-0.47719;,
 0.00022;6.74161;-0.49525;,
 4.03732;5.19521;-0.53529;,
 5.43782;2.08327;-0.53970;,
 5.97679;-2.08327;-0.56660;,
 5.86008;-10.51414;-0.58738;,
 3.00409;-5.14154;-0.59214;,
 -0.00022;-4.16914;-0.55720;,
 -3.00160;-5.15876;-0.55198;,
 -5.83992;-10.62651;-0.52309;,
 -5.59154;-1.97174;-2.56604;,
 -5.62279;-6.89402;-2.60675;,
 -5.31926;1.99088;-2.54784;,
 -4.10147;4.57496;-2.55926;,
 -0.01478;6.42123;-2.57999;,
 4.07179;4.57496;-2.61806;,
 5.28962;1.99088;-2.62417;,
 5.56161;-1.97174;-2.64629;,
 5.61299;-6.78165;-2.66770;,
 3.17610;-5.06506;-2.67477;,
 -0.01520;-3.82961;-2.63891;,
 -3.20274;-5.08672;-2.63271;,
 -5.62279;-6.89402;-2.60675;,
 -5.12160;-1.66719;-4.45548;,
 -4.96270;-9.56666;-4.48733;,
 -4.73766;1.70361;-4.43995;,
 -4.12904;3.29445;-4.44102;,
 -0.02833;5.14373;-4.46369;,
 4.07228;3.29445;-4.50003;,
 4.68086;1.70361;-4.50772;,
 5.06453;-1.66720;-4.52877;,
 4.92591;-9.45430;-4.53859;,
 3.50807;-4.76660;-4.45327;,
 -0.02889;-2.86340;-4.37602;,
 -3.56000;-4.79905;-4.40816;,
 -4.96270;-9.56666;-4.48733;,
 -4.55131;-1.19946;-4.63778;,
 -2.82864;-3.18077;-4.65927;,
 -4.55122;1.24958;-4.62652;,
 -2.82843;3.23089;-4.62981;,
 -0.04097;3.98768;-4.64639;,
 2.74643;3.23089;-4.66993;,
 4.46910;1.24958;-4.69143;,
 4.46901;-1.19946;-4.70268;,
 2.74622;-3.18077;-4.69939;,
 -0.04104;-2.22025;-4.81931;,
 -2.82864;-3.18077;-4.65927;,
 -2.42039;-0.61431;-4.72728;,
 -1.51474;-1.65594;-4.73858;,
 -2.42035;0.67323;-4.72136;,
 -1.51463;1.71486;-4.72309;,
 -0.04917;2.11273;-4.73181;,
 1.41625;1.71486;-4.74418;,
 2.32191;0.67323;-4.75548;,
 2.32187;-0.61431;-4.76140;,
 1.41614;-1.65594;-4.75967;,
 -0.04891;-0.57608;-5.03231;,
 -1.51474;-1.65594;-4.73858;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 0.04265;-0.03098;5.40094;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;,
 -0.05162;0.03098;-5.07133;;
 
 128;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;17,16,18;,
 3;18,16,19;,
 3;16,20,19;,
 3;20,21,19;,
 3;19,21,22;,
 3;21,23,22;,
 3;3,2,24;,
 3;2,25,24;,
 3;25,26,24;,
 4;2,5,27,25;,
 4;5,7,28,27;,
 4;7,9,29,28;,
 4;9,11,30,29;,
 4;11,13,31,30;,
 4;13,15,32,31;,
 3;32,15,33;,
 3;33,15,34;,
 3;15,17,34;,
 3;34,17,33;,
 3;33,17,35;,
 3;17,18,35;,
 3;18,19,35;,
 3;19,22,35;,
 3;35,22,36;,
 3;22,23,36;,
 3;23,37,36;,
 4;26,25,38,39;,
 4;25,27,40,38;,
 4;27,28,41,40;,
 4;28,29,42,41;,
 4;29,30,43,42;,
 4;30,31,44,43;,
 4;31,32,45,44;,
 4;32,33,46,45;,
 3;33,35,46;,
 3;46,35,47;,
 3;35,48,47;,
 3;48,35,49;,
 3;49,35,50;,
 3;35,36,50;,
 4;39,38,51,52;,
 4;38,40,53,51;,
 4;40,41,54,53;,
 4;41,42,55,54;,
 4;42,43,56,55;,
 4;43,44,57,56;,
 4;44,45,58,57;,
 4;45,46,59,58;,
 4;60,47,48,61;,
 4;62,49,50,63;,
 4;52,51,64,65;,
 4;51,53,66,64;,
 4;53,54,67,66;,
 4;54,55,68,67;,
 4;55,56,69,68;,
 4;56,57,70,69;,
 4;57,58,71,70;,
 4;58,59,72,71;,
 4;73,60,61,74;,
 4;75,62,63,76;,
 4;65,64,77,78;,
 4;64,66,79,77;,
 4;66,67,80,79;,
 4;67,68,81,80;,
 4;68,69,82,81;,
 4;69,70,83,82;,
 4;70,71,84,83;,
 4;71,72,85,84;,
 4;86,73,74,87;,
 4;88,75,76,89;,
 4;78,77,90,91;,
 4;77,79,92,90;,
 4;79,80,93,92;,
 4;80,81,94,93;,
 4;81,82,95,94;,
 4;82,83,96,95;,
 4;83,84,97,96;,
 4;84,85,98,97;,
 3;85,86,98;,
 3;86,87,98;,
 3;87,99,98;,
 3;99,87,100;,
 3;87,88,100;,
 3;88,89,100;,
 4;91,90,101,102;,
 4;90,92,103,101;,
 4;92,93,104,103;,
 4;93,94,105,104;,
 4;94,95,106,105;,
 4;95,96,107,106;,
 4;96,97,108,107;,
 4;97,98,109,108;,
 4;98,99,110,109;,
 4;99,100,111,110;,
 3;1,0,112;,
 3;4,1,113;,
 3;6,4,114;,
 3;8,6,115;,
 3;10,8,116;,
 3;12,10,117;,
 3;14,12,118;,
 3;16,14,119;,
 3;20,16,120;,
 3;21,20,121;,
 3;102,101,122;,
 3;101,103,123;,
 3;103,104,124;,
 3;104,105,125;,
 3;105,106,126;,
 3;106,107,127;,
 3;107,108,128;,
 3;108,109,129;,
 3;109,110,130;,
 3;110,111,131;,
 4;74,75,88,87;,
 4;61,62,75,74;,
 4;48,49,62,61;,
 4;72,73,86,85;,
 4;59,60,73,72;,
 4;46,47,60,59;;
 
 MeshMaterialList {
  1;
  128;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
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
    "data\\TEXTURE\\neet2.jpg";
   }
  }
 }
 MeshNormals {
  129;
  -0.269442;-0.020597;0.962796;,
  -0.232563;-0.042517;0.971652;,
  -0.232593;0.113152;0.965970;,
  -0.150312;0.214975;0.964983;,
  0.007016;0.223204;0.974747;,
  0.164184;0.214976;0.962720;,
  0.246470;0.113156;0.962522;,
  0.246522;-0.042515;0.968204;,
  0.284248;-0.021110;0.958518;,
  0.009474;0.020410;0.999747;,
  -0.498471;-0.046562;0.865655;,
  -0.578986;-0.022742;0.815020;,
  -0.560834;0.198996;0.803658;,
  -0.344506;0.409992;0.844525;,
  0.006290;0.485505;0.874211;,
  0.356623;0.409991;0.839481;,
  0.572341;0.198996;0.795504;,
  0.549031;-0.022445;0.835501;,
  0.504650;-0.046143;0.862090;,
  0.009399;0.000279;0.999956;,
  -0.723715;-0.007121;0.690062;,
  -0.786694;0.031067;0.616560;,
  -0.792664;0.268305;0.547445;,
  -0.512583;0.594799;0.619251;,
  0.004736;0.752838;0.658189;,
  0.521442;0.594799;0.611811;,
  0.800459;0.268305;0.535983;,
  0.847478;0.053765;0.528101;,
  0.671162;-0.021534;0.740998;,
  -0.163366;-0.488318;-0.857238;,
  -0.974136;-0.006964;0.225856;,
  -0.972229;0.050582;0.228502;,
  -0.918594;0.287190;0.271492;,
  -0.618074;0.719597;0.316487;,
  0.002213;0.951523;0.307570;,
  0.622565;0.719597;0.307560;,
  0.922405;0.287190;0.258246;,
  0.975331;0.052270;0.214472;,
  0.977301;-0.003594;0.211824;,
  0.000126;-0.910664;-0.413147;,
  -0.999971;-0.005137;0.005552;,
  -0.998822;0.047759;0.008578;,
  -0.962195;0.270905;0.028121;,
  -0.680764;0.732480;0.005794;,
  -0.000277;0.999258;-0.038523;,
  0.680777;0.732480;-0.004003;,
  0.962500;0.270906;0.014273;,
  0.998768;0.049281;-0.005766;,
  0.999959;-0.002068;-0.008779;,
  0.002830;-0.999816;-0.018959;,
  -0.976469;-0.004317;-0.215613;,
  -0.977777;0.044680;-0.204833;,
  -0.948993;0.248620;-0.193908;,
  -0.681184;0.669358;-0.296561;,
  -0.002845;0.918475;-0.395470;,
  0.676845;0.669359;-0.306331;,
  0.946105;0.248620;-0.207543;,
  0.974729;0.045964;-0.218613;,
  0.973393;-0.001749;-0.229134;,
  0.001668;-0.971007;-0.239046;,
  -0.961585;-0.004873;-0.274463;,
  -0.857450;0.044199;-0.512665;,
  -0.710828;0.185337;-0.678509;,
  -0.439750;0.433618;-0.786508;,
  -0.006278;0.549624;-0.835389;,
  0.426885;0.432137;-0.794372;,
  0.694978;0.184124;-0.695057;,
  0.706955;0.014088;-0.707118;,
  0.957337;-0.002487;-0.288962;,
  -0.004992;-0.609939;-0.792433;,
  0.002932;-0.190799;-0.981625;,
  -0.173849;-0.013123;-0.984685;,
  -0.195247;0.058595;-0.979002;,
  -0.091194;0.111654;-0.989554;,
  -0.007383;0.128951;-0.991623;,
  0.075129;0.109828;-0.991107;,
  0.173017;0.056754;-0.983282;,
  0.152695;-0.013435;-0.988182;,
  -0.017930;-0.193295;-0.980977;,
  -0.006958;-0.327351;-0.944877;,
  -0.096373;-0.060864;-0.993483;,
  -0.085892;-0.026006;-0.995965;,
  -0.085910;0.035183;-0.995682;,
  -0.055780;0.084605;-0.994852;,
  -0.007163;0.103405;-0.994613;,
  0.041410;0.084545;-0.995559;,
  0.071468;0.035145;-0.996824;,
  0.071446;-0.025970;-0.997106;,
  0.082502;-0.060351;-0.994762;,
  -0.006704;-0.065036;-0.997860;,
  0.007189;0.039958;0.999175;,
  -0.007061;0.022447;-0.999723;,
  0.735346;-0.645169;0.207419;,
  0.586151;-0.794663;-0.157916;,
  0.584136;-0.810504;-0.043221;,
  0.324586;-0.471663;-0.819865;,
  -0.302988;-0.607359;-0.734380;,
  -0.577212;-0.802353;-0.151842;,
  -0.574800;-0.817518;-0.035625;,
  -0.329386;-0.477518;-0.814543;,
  0.322439;0.017778;0.946423;,
  -0.310395;0.018153;0.950434;,
  0.542020;-0.038717;0.839473;,
  -0.611391;-0.029724;0.790770;,
  -0.445825;-0.465902;-0.764313;,
  -0.490084;-0.512869;-0.704828;,
  -0.788976;-0.300050;0.536178;,
  0.676485;-0.330699;-0.658033;,
  -0.685313;-0.329445;-0.649470;,
  0.667368;-0.682538;0.297928;,
  0.570120;-0.095636;-0.815976;,
  0.491467;-0.257656;-0.831909;,
  -0.394975;-0.385408;-0.833940;,
  0.392364;-0.377639;-0.838713;,
  -0.596637;-0.783431;0.173958;,
  0.607299;-0.777590;0.162919;,
  0.800918;-0.548972;0.239082;,
  0.785705;-0.615292;-0.063900;,
  0.770401;-0.632599;-0.079383;,
  0.783602;-0.424224;0.453874;,
  -0.137202;-0.074864;-0.987710;,
  0.121369;-0.074319;-0.989821;,
  -0.760784;-0.432528;0.483866;,
  -0.680996;-0.218694;0.698869;,
  0.121549;-0.205612;-0.971056;,
  0.718176;-0.222248;0.659416;,
  -0.758167;-0.648153;-0.071282;,
  -0.775201;-0.629379;-0.054269;,
  -0.788483;-0.559924;0.254518;;
  128;
  4;0,1,11,10;,
  4;1,2,12,11;,
  4;2,3,13,12;,
  4;3,4,14,13;,
  4;4,5,15,14;,
  4;5,6,16,15;,
  4;6,7,17,16;,
  4;7,8,18,17;,
  3;18,8,100;,
  3;100,8,19;,
  3;8,9,19;,
  3;9,0,19;,
  3;19,0,101;,
  3;0,10,101;,
  3;10,11,103;,
  3;11,21,103;,
  3;21,20,103;,
  4;11,12,22,21;,
  4;12,13,23,22;,
  4;13,14,24,23;,
  4;14,15,25,24;,
  4;15,16,26,25;,
  4;16,17,27,26;,
  3;27,17,28;,
  3;28,17,102;,
  3;17,18,102;,
  3;102,18,28;,
  3;104,105,29;,
  3;106,106,106;,
  3;107,107,107;,
  3;108,108,29;,
  3;109,109,109;,
  3;110,110,111;,
  3;10,103,20;,
  4;20,21,31,30;,
  4;21,22,32,31;,
  4;22,23,33,32;,
  4;23,24,34,33;,
  4;24,25,35,34;,
  4;25,26,36,35;,
  4;26,27,37,36;,
  4;27,28,38,37;,
  3;104,29,112;,
  3;112,29,99;,
  3;29,39,99;,
  3;39,29,95;,
  3;95,29,113;,
  3;29,111,113;,
  4;30,31,41,40;,
  4;31,32,42,41;,
  4;32,33,43,42;,
  4;33,34,44,43;,
  4;34,35,45,44;,
  4;35,36,46,45;,
  4;36,37,47,46;,
  4;37,38,48,47;,
  4;98,114,39,49;,
  4;94,115,116,117;,
  4;40,41,51,50;,
  4;41,42,52,51;,
  4;42,43,53,52;,
  4;43,44,54,53;,
  4;44,45,55,54;,
  4;45,46,56,55;,
  4;46,47,57,56;,
  4;47,48,58,57;,
  4;97,98,49,59;,
  4;93,94,117,118;,
  4;50,51,61,60;,
  4;51,52,62,61;,
  4;52,53,63,62;,
  4;53,54,64,63;,
  4;54,55,65,64;,
  4;55,56,66,65;,
  4;56,57,67,66;,
  4;57,58,68,67;,
  4;96,97,59,69;,
  4;92,93,118,119;,
  4;120,120,71,70;,
  4;61,62,72,71;,
  4;62,63,73,72;,
  4;63,64,74,73;,
  4;64,65,75,74;,
  4;65,66,76,75;,
  4;66,67,77,76;,
  4;67,121,78,77;,
  3;122,122,123;,
  3;96,69,78;,
  3;69,79,78;,
  3;79,69,70;,
  3;69,124,70;,
  3;92,119,125;,
  4;70,71,81,80;,
  4;71,72,82,81;,
  4;72,73,83,82;,
  4;73,74,84,83;,
  4;74,75,85,84;,
  4;75,76,86,85;,
  4;76,77,87,86;,
  4;77,78,88,87;,
  4;78,79,89,88;,
  4;79,70,80,89;,
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
  3;82,83,91;,
  3;83,84,91;,
  3;84,85,91;,
  3;85,86,91;,
  3;86,87,91;,
  3;87,88,91;,
  3;88,89,91;,
  3;89,80,91;,
  4;59,93,92,69;,
  4;49,94,93,59;,
  4;39,115,94,49;,
  4;126,97,122,122;,
  4;127,98,97,126;,
  4;128,114,98,127;;
 }
 MeshTextureCoords {
  132;
  0.819127;0.170264;,
  0.835551;0.170264;,
  0.835551;0.153841;,
  0.819127;0.153841;,
  0.851974;0.170264;,
  0.851974;0.153841;,
  0.868398;0.170264;,
  0.868398;0.153841;,
  0.884822;0.170264;,
  0.884822;0.153841;,
  0.901246;0.170264;,
  0.901246;0.153841;,
  0.917669;0.170264;,
  0.917669;0.153841;,
  0.934093;0.170264;,
  0.934093;0.153841;,
  0.950517;0.170264;,
  0.950517;0.153841;,
  0.957943;0.153841;,
  0.966941;0.153841;,
  0.966941;0.170264;,
  0.983364;0.170264;,
  0.975938;0.153841;,
  0.983364;0.153841;,
  0.819127;0.144741;,
  0.835551;0.137417;,
  0.819127;0.137417;,
  0.851974;0.137417;,
  0.868398;0.137417;,
  0.884822;0.137417;,
  0.901246;0.137417;,
  0.917669;0.137417;,
  0.934093;0.137417;,
  0.950517;0.137417;,
  0.950517;0.144741;,
  0.966941;0.137417;,
  0.983364;0.137417;,
  0.983364;0.144741;,
  0.835551;0.120993;,
  0.819127;0.120993;,
  0.851974;0.120993;,
  0.868398;0.120993;,
  0.884822;0.120993;,
  0.901246;0.120993;,
  0.917669;0.120993;,
  0.934093;0.120993;,
  0.950517;0.120993;,
  0.963738;0.120993;,
  0.966941;0.120993;,
  0.970143;0.120993;,
  0.983364;0.120993;,
  0.835551;0.104570;,
  0.819127;0.104570;,
  0.851974;0.104570;,
  0.868398;0.104570;,
  0.884822;0.104570;,
  0.901246;0.104570;,
  0.917669;0.104570;,
  0.934093;0.104570;,
  0.950517;0.104570;,
  0.964423;0.104570;,
  0.966941;0.104570;,
  0.969458;0.104570;,
  0.983364;0.104570;,
  0.835551;0.088146;,
  0.819127;0.088146;,
  0.851974;0.088146;,
  0.868398;0.088146;,
  0.884822;0.088146;,
  0.901246;0.088146;,
  0.917669;0.088146;,
  0.934093;0.088146;,
  0.950517;0.088146;,
  0.963774;0.088146;,
  0.966941;0.088146;,
  0.970107;0.088146;,
  0.983364;0.088146;,
  0.835551;0.071722;,
  0.819127;0.071722;,
  0.851974;0.071722;,
  0.868398;0.071722;,
  0.884822;0.071722;,
  0.901246;0.071722;,
  0.917669;0.071722;,
  0.934093;0.071722;,
  0.950517;0.071722;,
  0.962198;0.071722;,
  0.966941;0.071722;,
  0.971683;0.071722;,
  0.983364;0.071722;,
  0.835551;0.055298;,
  0.819127;0.055298;,
  0.851974;0.055298;,
  0.868398;0.055298;,
  0.884822;0.055298;,
  0.901246;0.055298;,
  0.917669;0.055298;,
  0.934093;0.055298;,
  0.950517;0.055298;,
  0.966941;0.055298;,
  0.983364;0.055298;,
  0.835551;0.038875;,
  0.819127;0.038875;,
  0.851974;0.038875;,
  0.868398;0.038875;,
  0.884822;0.038875;,
  0.901246;0.038875;,
  0.917669;0.038875;,
  0.934093;0.038875;,
  0.950517;0.038875;,
  0.966941;0.038875;,
  0.983364;0.038875;,
  0.827339;0.186688;,
  0.843763;0.186688;,
  0.860186;0.186688;,
  0.876610;0.186688;,
  0.893034;0.186688;,
  0.909457;0.186688;,
  0.925881;0.186688;,
  0.942305;0.186688;,
  0.958729;0.186688;,
  0.975152;0.186688;,
  0.827339;0.022451;,
  0.843763;0.022451;,
  0.860186;0.022451;,
  0.876610;0.022451;,
  0.893034;0.022451;,
  0.909457;0.022451;,
  0.925881;0.022451;,
  0.942305;0.022451;,
  0.958729;0.022451;,
  0.975152;0.022451;;
 }
}
