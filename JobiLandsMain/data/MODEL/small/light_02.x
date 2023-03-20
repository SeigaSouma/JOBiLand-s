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
 149;
 0.40494;397.55770;-4.01644;,
 3.52561;397.55770;-2.80497;,
 3.52561;-1.16017;-3.06378;,
 0.40494;-1.16017;-4.38244;,
 4.81825;397.55770;0.11980;,
 4.81825;-1.16017;0.11980;,
 3.52561;397.55770;3.04457;,
 3.52561;-1.16017;3.30337;,
 0.40494;397.55770;4.25604;,
 0.40494;-1.16017;4.62204;,
 -2.71577;397.55770;3.04455;,
 -2.71577;-1.16017;3.30336;,
 -4.00840;397.55770;0.11980;,
 -4.00840;-1.16017;0.11980;,
 -2.71577;397.55770;-2.80497;,
 -2.71577;-1.16017;-3.06378;,
 0.40494;397.55770;-4.01644;,
 0.40494;-1.16017;-4.38244;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;397.55770;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 0.40494;-1.16017;0.11980;,
 37.32152;397.16785;21.68270;,
 67.20820;396.82850;12.11843;,
 67.20820;407.12484;11.76431;,
 36.85580;407.50350;21.43689;,
 68.13873;396.40692;0.22368;,
 68.13873;406.70355;-0.13058;,
 67.20820;395.92816;-13.27703;,
 67.20820;406.22523;-13.63132;,
 37.32152;395.63641;-21.50340;,
 37.32152;405.93372;-21.85760;,
 -5.88243;395.95701;-10.81145;,
 -6.14094;396.40692;11.52884;,
 -6.14094;406.70355;10.92453;,
 -5.88243;406.25335;-11.11796;,
 -6.19301;396.86385;11.33662;,
 -6.19301;406.58250;11.03016;,
 16.49910;395.95345;-10.88525;,
 15.40316;396.62723;11.32401;,
 37.32152;396.40692;0.22368;,
 37.32152;396.40692;0.22368;,
 37.32152;397.16785;21.68270;,
 15.25523;396.88055;11.74612;,
 15.40316;396.62723;11.32401;,
 15.25523;407.17664;13.23345;,
 36.85580;407.50350;21.43689;,
 16.48334;406.25115;-12.88340;,
 37.32152;396.40692;0.22368;,
 37.32152;396.40692;0.22368;,
 36.92554;406.70355;-0.13058;,
 36.92554;406.70355;-0.13058;,
 15.46711;406.70355;-0.13058;,
 36.92554;406.70355;-0.13058;,
 15.12132;407.21854;13.39475;,
 15.12132;407.21854;13.39475;,
 15.46711;406.70355;-0.13058;,
 36.92554;406.70355;-0.13058;,
 44.62217;393.58045;0.42712;,
 51.95960;393.94007;4.65638;,
 44.62217;394.00198;6.41096;,
 44.62217;393.58045;0.42712;,
 54.99880;393.78953;0.41993;,
 44.62217;393.58045;0.42712;,
 51.95961;393.63953;-3.81682;,
 44.62217;393.58045;0.42712;,
 44.62224;393.57679;-5.57148;,
 44.62217;393.58045;0.42712;,
 37.28494;393.63953;-3.81665;,
 44.62217;393.58045;0.42712;,
 34.24575;393.78953;0.41993;,
 44.62217;393.58045;0.42712;,
 37.28494;393.94007;4.65630;,
 44.62217;393.58045;0.42712;,
 44.62217;394.00198;6.41096;,
 58.17980;394.66229;8.22704;,
 44.62217;394.77669;11.46970;,
 63.79549;394.38538;0.39949;,
 58.17979;394.10784;-7.42871;,
 44.62227;393.99275;-10.67091;,
 31.06453;394.10775;-7.42871;,
 25.44895;394.38538;0.39941;,
 31.06470;394.66229;8.22719;,
 44.62217;394.77669;11.46970;,
 62.33615;395.63851;10.59627;,
 44.62232;395.78821;14.83284;,
 63.75515;395.27552;0.36874;,
 62.33604;394.91336;-9.85902;,
 44.62236;394.76312;-14.09564;,
 26.90840;394.91336;-9.85902;,
 19.57117;395.27552;0.36859;,
 26.90840;395.63851;10.59634;,
 44.62232;395.78821;14.83284;,
 63.79546;396.71912;11.40281;,
 44.62217;396.88172;15.98838;,
 65.81914;396.32703;0.33260;,
 63.79561;395.93475;-10.73771;,
 44.62217;395.77186;-15.32344;,
 25.44896;395.93460;-10.73790;,
 17.50714;396.32703;0.27592;,
 25.44889;396.71951;11.40279;,
 44.62217;396.88172;15.98838;,
 62.33615;397.74056;10.52403;,
 44.62232;397.89012;14.76053;,
 63.75515;397.37781;0.29647;,
 62.33604;397.01522;-9.93124;,
 44.62236;396.86478;-14.16792;,
 26.90840;397.01522;-9.93124;,
 19.57117;397.37781;0.29622;,
 26.90840;397.74056;10.52403;,
 44.62232;397.89012;14.76053;,
 58.17980;398.54612;8.09354;,
 44.62217;398.66102;11.33606;,
 57.87742;398.26873;0.26566;,
 58.17979;397.99085;-7.56227;,
 44.62227;397.87586;-10.80452;,
 31.06453;397.99085;-7.56227;,
 25.44895;398.26859;0.26568;,
 31.06470;398.54612;8.09360;,
 44.62217;398.66102;11.33606;,
 51.95960;399.01390;4.48172;,
 44.62217;399.07586;6.23648;,
 54.99880;398.86351;0.24532;,
 51.95961;398.71375;-3.99139;,
 44.62224;398.65145;-5.74617;,
 37.28494;398.71375;-3.99123;,
 34.24575;398.86351;0.24529;,
 37.28494;399.01390;4.48172;,
 44.62217;399.07586;6.23648;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;,
 44.62217;399.07337;0.23804;;
 
 112;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,16,17,15;,
 3;18,1,0;,
 3;19,4,1;,
 3;20,6,4;,
 3;21,8,6;,
 3;22,10,8;,
 3;23,12,10;,
 3;24,14,12;,
 3;25,16,14;,
 3;26,3,2;,
 3;27,2,5;,
 3;28,5,7;,
 3;29,7,9;,
 3;30,9,11;,
 3;31,11,13;,
 3;32,13,15;,
 3;33,15,17;,
 4;34,35,36,37;,
 4;35,38,39,36;,
 4;38,40,41,39;,
 4;40,42,43,41;,
 4;44,45,46,47;,
 4;45,48,49,46;,
 4;44,50,51,45;,
 4;51,50,42,52;,
 4;53,54,55,56;,
 4;45,56,55,48;,
 4;57,49,48,55;,
 4;54,58,57,55;,
 4;42,50,59,43;,
 4;47,59,50,44;,
 4;38,60,42,40;,
 4;38,35,34,61;,
 4;37,36,39,62;,
 4;39,41,43,63;,
 4;43,59,64,65;,
 4;46,64,59,47;,
 3;66,57,58;,
 3;67,68,46;,
 4;58,69,68,67;,
 4;46,49,57,66;,
 3;70,71,72;,
 3;73,74,71;,
 3;75,76,74;,
 3;77,78,76;,
 3;79,80,78;,
 3;81,82,80;,
 3;83,84,82;,
 3;85,86,84;,
 4;72,71,87,88;,
 4;71,74,89,87;,
 4;74,76,90,89;,
 4;76,78,91,90;,
 4;78,80,92,91;,
 4;80,82,93,92;,
 4;82,84,94,93;,
 4;84,86,95,94;,
 4;88,87,96,97;,
 4;87,89,98,96;,
 4;89,90,99,98;,
 4;90,91,100,99;,
 4;91,92,101,100;,
 4;92,93,102,101;,
 4;93,94,103,102;,
 4;94,95,104,103;,
 4;97,96,105,106;,
 4;96,98,107,105;,
 4;98,99,108,107;,
 4;99,100,109,108;,
 4;100,101,110,109;,
 4;101,102,111,110;,
 4;102,103,112,111;,
 4;103,104,113,112;,
 4;106,105,114,115;,
 4;105,107,116,114;,
 4;107,108,117,116;,
 4;108,109,118,117;,
 4;109,110,119,118;,
 4;110,111,120,119;,
 4;111,112,121,120;,
 4;112,113,122,121;,
 4;115,114,123,124;,
 4;114,116,125,123;,
 4;116,117,126,125;,
 4;117,118,127,126;,
 4;118,119,128,127;,
 4;119,120,129,128;,
 4;120,121,130,129;,
 4;121,122,131,130;,
 4;124,123,132,133;,
 4;123,125,134,132;,
 4;125,126,135,134;,
 4;126,127,136,135;,
 4;127,128,137,136;,
 4;128,129,138,137;,
 4;129,130,139,138;,
 4;130,131,140,139;,
 3;133,132,141;,
 3;132,134,142;,
 3;134,135,143;,
 3;135,136,144;,
 3;136,137,145;,
 3;137,138,146;,
 3;138,139,147;,
 3;139,140,148;;
 
 MeshMaterialList {
  3;
  112;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  2,
  0,
  0,
  0,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
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
   0.420000;0.222400;0.140800;1.000000;;
   5.000000;
   0.060000;0.060000;0.060000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\small\\metal02.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;0.450000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.730000;0.730000;0.730000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  114;
  0.000000;1.000000;0.000000;,
  0.000003;0.000784;-1.000000;,
  0.701473;0.000459;-0.712696;,
  1.000000;0.000138;0.000000;,
  0.701474;0.000464;0.712695;,
  0.000001;0.000784;1.000000;,
  -0.701473;0.000459;0.712696;,
  -1.000000;0.000133;0.000001;,
  -0.701472;0.000459;-0.712697;,
  0.000000;-1.000000;-0.000000;,
  -0.022957;-0.995720;0.089528;,
  -0.041068;-0.014853;0.999046;,
  0.304404;0.034534;0.951917;,
  0.999989;0.000159;0.004622;,
  0.997636;-0.002363;-0.068678;,
  -0.084502;-0.071805;-0.993833;,
  -0.999932;-0.000516;-0.011638;,
  0.083635;-0.042139;-0.995605;,
  0.000155;0.999346;-0.036156;,
  0.004665;-0.999564;0.029152;,
  -0.028912;-0.994132;0.104237;,
  -0.155540;-0.401956;0.902352;,
  -0.182126;-0.135512;0.973893;,
  -0.236875;-0.108036;-0.965515;,
  -0.006510;0.999369;-0.034911;,
  -0.062051;-0.977350;0.202328;,
  -0.000000;-0.999371;0.035455;,
  -0.000000;-0.993830;0.110911;,
  0.030996;-0.995560;0.088883;,
  0.043866;-0.998410;0.035390;,
  0.031030;-0.999353;-0.018212;,
  0.000001;-0.999186;-0.040348;,
  -0.031027;-0.999353;-0.018207;,
  -0.043864;-0.998410;0.035393;,
  -0.030996;-0.995560;0.088883;,
  0.000000;-0.975460;0.220175;,
  0.218683;-0.942734;0.251853;,
  0.412685;-0.910304;0.032212;,
  0.218788;-0.958152;-0.184601;,
  0.000001;-0.988583;-0.150676;,
  -0.076572;-0.992363;-0.096707;,
  -0.108729;-0.993448;0.035206;,
  -0.076559;-0.983038;0.166658;,
  -0.000005;-0.865980;0.500078;,
  0.368930;-0.838479;0.401053;,
  0.616875;-0.786569;0.027833;,
  0.369116;-0.864578;-0.340967;,
  0.000005;-0.899017;-0.437915;,
  -0.206014;-0.928187;-0.309884;,
  -0.302664;-0.952473;0.034483;,
  -0.207632;-0.903276;0.375475;,
  -0.000010;-0.726386;0.687287;,
  0.348243;-0.837998;0.420104;,
  0.500732;-0.865060;0.030646;,
  0.348452;-0.865365;-0.360173;,
  0.000010;-0.772646;-0.634837;,
  -0.300568;-0.832941;-0.464615;,
  -0.454453;-0.890159;0.033009;,
  -0.303613;-0.796110;0.523477;,
  0.000005;0.899017;0.437914;,
  0.234011;0.944692;0.229774;,
  0.341464;0.939305;-0.033291;,
  0.233860;0.926182;-0.295799;,
  0.000001;0.866059;-0.499942;,
  -0.205663;0.904170;-0.374404;,
  -0.302239;0.952662;-0.032961;,
  -0.207531;0.927578;0.310692;,
  -0.000001;0.988588;0.150646;,
  0.099689;0.992847;0.065707;,
  0.153903;0.987465;-0.035016;,
  0.099693;0.985704;-0.135829;,
  -0.000000;0.975452;-0.220211;,
  -0.076543;0.983036;-0.166676;,
  -0.108729;0.993447;-0.035221;,
  -0.076548;0.992368;0.096677;,
  -0.000000;0.999185;0.040370;,
  0.048034;0.998782;0.011248;,
  0.077933;0.996333;-0.035310;,
  0.048052;0.995478;-0.081940;,
  -0.000001;0.993817;-0.111033;,
  -0.031010;0.995557;-0.088904;,
  -0.043891;0.998409;-0.035385;,
  -0.030991;0.999354;0.018188;,
  -0.000000;0.999373;-0.035398;,
  0.996958;0.002680;0.077897;,
  0.265536;-0.033147;-0.963531;,
  0.005179;-0.999667;0.025288;,
  0.002076;-0.999412;0.034228;,
  -0.048087;-0.988278;0.144895;,
  -0.002798;-0.309773;0.950806;,
  -0.030891;-0.184325;0.982380;,
  -0.078499;-0.169880;0.982333;,
  -0.040448;-0.110329;-0.993072;,
  -0.000003;-0.999372;0.035434;,
  0.000000;-0.999372;0.035440;,
  -0.000006;-0.999372;0.035429;,
  0.000317;0.999319;-0.036893;,
  0.000328;0.999344;-0.036229;,
  0.000164;0.999358;-0.035819;,
  -0.000001;0.999373;-0.035408;,
  -0.000007;0.999373;-0.035419;,
  -0.003313;0.999490;-0.031769;,
  -0.013167;0.999358;-0.033326;,
  -0.006614;0.999583;-0.028106;,
  -0.009708;0.999229;-0.038052;,
  -0.097484;-0.281793;0.954510;,
  0.000013;0.772706;0.634764;,
  0.348383;0.865413;0.360124;,
  0.500599;0.865137;-0.030643;,
  0.348117;0.838093;-0.420017;,
  0.000002;0.726526;-0.687139;,
  -0.299927;0.798661;-0.521713;,
  -0.453598;0.890703;-0.029941;,
  -0.303416;0.831151;0.465968;;
  112;
  4;1,2,2,1;,
  4;2,3,3,2;,
  4;3,4,4,3;,
  4;4,5,5,4;,
  4;5,6,6,5;,
  4;6,7,7,6;,
  4;7,8,8,7;,
  4;8,1,1,8;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  3;9,9,9;,
  4;11,12,12,11;,
  4;84,13,13,84;,
  4;13,14,14,13;,
  4;85,15,15,85;,
  4;16,16,16,16;,
  4;17,17,17,17;,
  4;86,19,20,86;,
  4;20,19,87,10;,
  4;10,88,21,20;,
  4;89,89,21,90;,
  4;22,91,90,21;,
  4;11,11,22,21;,
  4;15,23,23,15;,
  4;92,23,23,92;,
  4;93,10,87,94;,
  4;93,95,88,10;,
  4;96,97,98,18;,
  4;98,99,100,18;,
  4;100,101,24,18;,
  4;102,24,101,103;,
  3;25,25,25;,
  3;104,24,102;,
  4;96,18,24,104;,
  4;105,91,22,105;,
  3;26,28,27;,
  3;26,29,28;,
  3;26,30,29;,
  3;26,31,30;,
  3;26,32,31;,
  3;26,33,32;,
  3;26,34,33;,
  3;26,27,34;,
  4;27,28,36,35;,
  4;28,29,37,36;,
  4;29,30,38,37;,
  4;30,31,39,38;,
  4;31,32,40,39;,
  4;32,33,41,40;,
  4;33,34,42,41;,
  4;34,27,35,42;,
  4;35,36,44,43;,
  4;36,37,45,44;,
  4;37,38,46,45;,
  4;38,39,47,46;,
  4;39,40,48,47;,
  4;40,41,49,48;,
  4;41,42,50,49;,
  4;42,35,43,50;,
  4;43,44,52,51;,
  4;44,45,53,52;,
  4;45,46,54,53;,
  4;46,47,55,54;,
  4;47,48,56,55;,
  4;48,49,57,56;,
  4;49,50,58,57;,
  4;50,43,51,58;,
  4;106,107,60,59;,
  4;107,108,61,60;,
  4;108,109,62,61;,
  4;109,110,63,62;,
  4;110,111,64,63;,
  4;111,112,65,64;,
  4;112,113,66,65;,
  4;113,106,59,66;,
  4;59,60,68,67;,
  4;60,61,69,68;,
  4;61,62,70,69;,
  4;62,63,71,70;,
  4;63,64,72,71;,
  4;64,65,73,72;,
  4;65,66,74,73;,
  4;66,59,67,74;,
  4;67,68,76,75;,
  4;68,69,77,76;,
  4;69,70,78,77;,
  4;70,71,79,78;,
  4;71,72,80,79;,
  4;72,73,81,80;,
  4;73,74,82,81;,
  4;74,67,75,82;,
  3;75,76,83;,
  3;76,77,83;,
  3;77,78,83;,
  3;78,79,83;,
  3;79,80,83;,
  3;80,81,83;,
  3;81,82,83;,
  3;82,75,83;;
 }
 MeshTextureCoords {
  149;
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.625000;1.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.062500;0.000000;,
  0.187500;0.000000;,
  0.312500;0.000000;,
  0.437500;0.000000;,
  0.562500;0.000000;,
  0.687500;0.000000;,
  0.812500;0.000000;,
  0.937500;0.000000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;,
  0.000000;0.000000;,
  0.125000;0.000000;,
  0.125000;1.000000;,
  0.000000;1.000000;,
  0.250000;0.000000;,
  0.250000;1.000000;,
  0.375000;0.000000;,
  0.375000;1.000000;,
  0.500000;0.000000;,
  0.500000;1.000000;,
  0.625000;0.000000;,
  0.750000;0.000000;,
  0.750000;1.000000;,
  0.625000;1.000000;,
  0.875000;0.000000;,
  0.875000;1.000000;,
  0.558680;0.000000;,
  0.656250;0.000000;,
  0.562500;0.000000;,
  0.812500;0.000000;,
  1.000000;0.000000;,
  0.937500;0.000000;,
  0.781250;0.000000;,
  0.937500;1.000000;,
  1.000000;1.000000;,
  0.562500;1.000000;,
  0.312500;0.000000;,
  0.062500;0.000000;,
  0.187500;1.000000;,
  0.437500;1.000000;,
  0.718750;1.000000;,
  0.687500;1.000000;,
  0.906250;1.000000;,
  0.843750;1.000000;,
  0.781250;1.000000;,
  0.937500;1.000000;,
  0.062500;0.000000;,
  0.125000;0.125000;,
  0.000000;0.125000;,
  0.187500;0.000000;,
  0.250000;0.125000;,
  0.312500;0.000000;,
  0.375000;0.125000;,
  0.437500;0.000000;,
  0.500000;0.125000;,
  0.562500;0.000000;,
  0.625000;0.125000;,
  0.687500;0.000000;,
  0.750000;0.125000;,
  0.812500;0.000000;,
  0.875000;0.125000;,
  0.937500;0.000000;,
  1.000000;0.125000;,
  0.125000;0.250000;,
  0.000000;0.250000;,
  0.250000;0.250000;,
  0.375000;0.250000;,
  0.500000;0.250000;,
  0.625000;0.250000;,
  0.750000;0.250000;,
  0.875000;0.250000;,
  1.000000;0.250000;,
  0.125000;0.375000;,
  0.000000;0.375000;,
  0.250000;0.375000;,
  0.375000;0.375000;,
  0.500000;0.375000;,
  0.625000;0.375000;,
  0.750000;0.375000;,
  0.875000;0.375000;,
  1.000000;0.375000;,
  0.125000;0.500000;,
  0.000000;0.500000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.875000;0.500000;,
  1.000000;0.500000;,
  0.125000;0.625000;,
  0.000000;0.625000;,
  0.250000;0.625000;,
  0.375000;0.625000;,
  0.500000;0.625000;,
  0.625000;0.625000;,
  0.750000;0.625000;,
  0.875000;0.625000;,
  1.000000;0.625000;,
  0.125000;0.750000;,
  0.000000;0.750000;,
  0.250000;0.750000;,
  0.375000;0.750000;,
  0.500000;0.750000;,
  0.625000;0.750000;,
  0.750000;0.750000;,
  0.875000;0.750000;,
  1.000000;0.750000;,
  0.125000;0.875000;,
  0.000000;0.875000;,
  0.250000;0.875000;,
  0.375000;0.875000;,
  0.500000;0.875000;,
  0.625000;0.875000;,
  0.750000;0.875000;,
  0.875000;0.875000;,
  1.000000;0.875000;,
  0.062500;1.000000;,
  0.187500;1.000000;,
  0.312500;1.000000;,
  0.437500;1.000000;,
  0.562500;1.000000;,
  0.687500;1.000000;,
  0.812500;1.000000;,
  0.937500;1.000000;;
 }
}