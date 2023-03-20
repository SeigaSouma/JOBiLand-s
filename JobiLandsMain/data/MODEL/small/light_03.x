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
 378;
 -12.21119;9.57731;12.42956;,
 12.57275;9.57731;12.25451;,
 12.57275;9.57731;-10.97349;,
 -12.21119;9.57731;-11.14855;,
 -13.90633;-0.77694;-11.75146;,
 14.09105;-0.77694;-11.75146;,
 13.65530;-0.77694;13.03248;,
 -13.95481;-0.77694;13.03248;,
 13.66667;0.17104;13.03248;,
 12.57275;9.57731;12.25451;,
 -12.21119;9.57731;12.42956;,
 -13.90302;0.17104;13.03248;,
 -13.95481;-0.77694;13.03248;,
 13.65530;-0.77694;13.03248;,
 13.66667;0.17104;13.03248;,
 14.09105;-0.77694;-11.75146;,
 14.06745;0.17104;-11.75146;,
 12.57275;9.57731;-10.97349;,
 -13.90302;0.17104;13.03248;,
 -12.21119;9.57731;-11.14855;,
 -13.90713;0.17104;-11.75146;,
 -13.90633;-0.77694;-11.75146;,
 14.06745;0.17104;-11.75146;,
 14.09105;-0.77694;-11.75146;,
 -13.90633;-0.77694;-11.75146;,
 -13.90713;0.17104;-11.75146;,
 -12.21119;9.57731;-11.14855;,
 12.57275;9.57731;-10.97349;,
 -7.31290;184.77071;-8.92052;,
 8.46579;184.77071;-8.92052;,
 8.46579;11.26665;-8.92052;,
 -7.31290;11.26665;-8.92052;,
 8.46579;186.44230;-7.24882;,
 8.46579;184.77071;-8.92052;,
 -7.31290;184.77071;-8.92052;,
 -7.31290;186.44230;-7.24882;,
 10.13749;11.26665;-7.24882;,
 10.13749;184.77071;-7.24882;,
 -7.31290;9.59492;-7.24882;,
 -7.31290;11.26665;-8.92052;,
 8.46579;11.26665;-8.92052;,
 8.46579;9.59492;-7.24882;,
 -8.98459;184.77071;-7.24882;,
 -8.98459;11.26665;-7.24882;,
 -8.98459;184.77071;-7.24882;,
 10.13749;184.77071;-7.24882;,
 10.13749;11.26665;-7.24882;,
 -8.98459;11.26665;-7.24882;,
 10.13749;184.77071;8.52983;,
 10.13749;11.26665;8.52983;,
 8.46579;186.44230;8.52983;,
 8.46579;186.44230;-7.24882;,
 8.46579;11.26665;10.20153;,
 8.46579;184.77071;10.20153;,
 8.46579;9.59492;-7.24882;,
 8.46579;9.59492;8.52983;,
 -7.31290;184.77071;10.20153;,
 -7.31290;11.26665;10.20153;,
 -7.31290;186.44230;8.52983;,
 -8.98459;11.26665;8.52983;,
 -8.98459;184.77071;8.52983;,
 -7.31290;9.59492;8.52983;,
 -8.98459;184.77071;8.52983;,
 -8.98459;184.77071;-7.24882;,
 -8.98459;11.26665;-7.24882;,
 -8.98459;11.26665;8.52983;,
 -7.31290;186.44230;-7.24882;,
 -7.31290;9.59492;8.52983;,
 -8.98459;11.26665;8.52983;,
 -7.31290;186.44230;8.52983;,
 8.46579;186.44230;8.52983;,
 8.46579;9.59492;8.52983;,
 12.05285;272.50166;9.57191;,
 12.05285;272.50166;11.82879;,
 12.05285;269.75181;11.82879;,
 12.05285;269.75181;9.57191;,
 12.05285;186.69502;9.57191;,
 12.05285;189.68311;9.57191;,
 12.05285;189.68311;11.82879;,
 12.05285;186.69502;11.82879;,
 12.05285;272.50166;-8.08507;,
 12.05285;269.75181;-8.08507;,
 12.05285;269.75181;-10.71441;,
 12.05285;272.50166;-10.71441;,
 12.05285;186.69502;-8.08507;,
 12.05285;186.69502;-10.71441;,
 12.05285;189.68311;-10.71441;,
 12.05285;189.68311;-8.08507;,
 9.21876;269.75181;-10.71441;,
 12.05285;269.75181;-10.71441;,
 12.05285;189.68311;-10.71441;,
 9.21876;189.68313;-10.71441;,
 -7.71163;269.75181;-10.71441;,
 -7.71163;272.50166;-10.71441;,
 9.19155;272.50166;-10.71441;,
 12.05285;272.50166;-10.71441;,
 12.05285;186.69502;-10.71441;,
 9.21515;186.69502;-10.71441;,
 -7.71163;186.69502;-10.71441;,
 -7.71163;189.68311;-10.71441;,
 9.19155;272.50166;-10.71441;,
 -7.71163;272.50166;-10.71441;,
 -7.71163;272.50166;-8.33603;,
 9.15493;272.50166;-8.12653;,
 12.05285;272.50166;-8.08507;,
 12.05285;272.50166;-10.71441;,
 9.21515;186.69502;-10.71441;,
 12.05285;186.69502;-10.71441;,
 12.05285;186.69502;-8.08507;,
 8.78914;186.69502;-8.13686;,
 -7.71163;186.69502;-8.39875;,
 -7.71163;186.69502;-10.71441;,
 -7.71163;272.50166;9.57191;,
 8.92869;272.50166;9.57191;,
 12.05285;272.50166;9.57191;,
 12.05285;186.69502;9.57191;,
 8.78914;186.69502;9.57191;,
 -7.71163;186.69502;9.57191;,
 -7.71163;272.50166;11.82879;,
 8.92732;272.50166;11.82879;,
 8.78914;186.69502;11.82879;,
 -7.71163;186.69502;11.82879;,
 8.92732;272.50166;11.82879;,
 -7.71163;272.50166;11.82879;,
 -7.71163;269.75181;11.82879;,
 8.78914;269.75181;11.82879;,
 12.05285;269.75181;11.82879;,
 12.05285;272.50166;11.82879;,
 8.78914;186.69502;11.82879;,
 12.05285;186.69502;11.82879;,
 12.05285;189.68311;11.82879;,
 8.78914;189.68313;11.82879;,
 -7.71163;189.68311;11.82879;,
 -7.71163;186.69502;11.82879;,
 -11.19106;269.75181;9.57191;,
 -11.19106;189.68311;9.57191;,
 -11.19106;189.68311;11.82879;,
 -11.19106;269.75181;11.82879;,
 -11.19106;272.50166;11.82879;,
 -11.19106;272.50166;9.57191;,
 -11.19106;186.69502;9.57191;,
 -11.19106;186.69502;11.82879;,
 -11.19106;269.75181;-8.42854;,
 -11.19106;269.75181;-10.71441;,
 -11.19106;189.68311;-10.71441;,
 -11.19106;189.68311;-8.42854;,
 -11.19106;272.50166;-8.37130;,
 -11.19106;272.50166;-10.71441;,
 -11.19106;186.69502;-10.71441;,
 -11.19106;186.69502;-8.44285;,
 -7.71163;269.75181;9.42429;,
 8.78914;269.75181;9.42429;,
 -7.71163;189.68311;9.42429;,
 8.78914;189.68313;9.42429;,
 9.21876;189.68313;-8.53890;,
 9.21876;269.75181;-8.53890;,
 -7.71163;189.68311;-8.53890;,
 -7.71163;269.75181;-8.53890;,
 10.22084;269.75181;-8.08507;,
 10.22084;269.75181;9.57191;,
 10.22084;189.68311;9.57191;,
 10.22084;189.68311;-8.08507;,
 -9.82057;269.75181;-8.42854;,
 -9.82057;189.68311;-8.42854;,
 -9.82057;189.68311;9.57191;,
 -9.82057;269.75181;9.57191;,
 10.14558;186.70559;1.85055;,
 13.09178;186.70559;1.85055;,
 13.09178;186.70559;-0.81591;,
 10.14558;186.70559;-0.81591;,
 10.14558;169.65583;-0.81591;,
 10.89433;169.65583;-0.81591;,
 10.89433;169.65583;1.85055;,
 10.14558;169.65583;1.85055;,
 13.09178;183.76576;-0.81591;,
 13.09178;186.70559;-0.81591;,
 13.09178;183.76576;1.85055;,
 10.89433;169.65583;-0.81591;,
 13.09178;183.76576;-0.81591;,
 10.89433;169.65583;-0.81591;,
 10.14558;169.65583;-0.81591;,
 10.14558;183.76576;-0.81591;,
 10.14558;186.70559;-0.81591;,
 13.09178;186.70559;-0.81591;,
 13.09178;183.76576;1.85055;,
 13.09178;186.70559;1.85055;,
 10.14558;186.70559;1.85055;,
 10.14558;183.76576;1.85055;,
 10.14558;169.65583;1.85055;,
 10.89433;169.65583;1.85055;,
 10.14558;183.76576;-0.81591;,
 10.14558;169.65583;-0.81591;,
 10.14558;183.76576;1.85055;,
 10.14558;186.70559;-0.81591;,
 -9.30726;186.62100;-0.61310;,
 -12.51404;186.62100;-0.58739;,
 -12.48870;186.62100;2.07893;,
 -9.28196;186.62100;2.05322;,
 -9.28196;169.57133;2.05322;,
 -10.09690;169.57133;2.05974;,
 -10.12224;169.57133;-0.60658;,
 -9.30726;169.57133;-0.61310;,
 -12.48870;183.68125;2.07893;,
 -12.48870;186.62100;2.07893;,
 -12.51404;183.68125;-0.58739;,
 -10.09690;169.57133;2.05974;,
 -12.48870;183.68125;2.07893;,
 -10.09690;169.57133;2.05974;,
 -9.28196;169.57133;2.05322;,
 -9.28196;183.68125;2.05322;,
 -9.28196;186.62100;2.05322;,
 -12.48870;186.62100;2.07893;,
 -12.51404;183.68125;-0.58739;,
 -12.51404;186.62100;-0.58739;,
 -9.30726;186.62100;-0.61310;,
 -9.30726;183.68125;-0.61310;,
 -9.30726;169.57133;-0.61310;,
 -10.12224;169.57133;-0.60658;,
 -9.28196;183.68125;2.05322;,
 -9.28196;169.57133;2.05322;,
 -9.30726;183.68125;-0.61310;,
 -9.28196;186.62100;2.05322;,
 -0.83822;186.63581;10.14436;,
 -0.94103;186.63581;13.08875;,
 1.72378;186.63581;13.18179;,
 1.82659;186.63581;10.23740;,
 1.82659;169.58605;10.23740;,
 1.80045;169.58605;10.98569;,
 -0.86437;169.58605;10.89264;,
 -0.83822;169.58605;10.14436;,
 1.72378;183.69597;13.18179;,
 1.72378;186.63581;13.18179;,
 -0.94103;183.69597;13.08875;,
 1.80045;169.58605;10.98569;,
 1.72378;183.69597;13.18179;,
 1.80045;169.58605;10.98569;,
 1.82659;169.58605;10.23740;,
 1.82659;183.69597;10.23740;,
 1.82659;186.63581;10.23740;,
 1.72378;186.63581;13.18179;,
 -0.94103;183.69597;13.08875;,
 -0.94103;186.63581;13.08875;,
 -0.83822;186.63581;10.14436;,
 -0.83822;183.69597;10.14436;,
 -0.83822;169.58605;10.14436;,
 -0.86437;169.58605;10.89264;,
 1.82659;183.69597;10.23740;,
 1.82659;169.58605;10.23740;,
 -0.83822;183.69597;10.14436;,
 1.82659;186.63581;10.23740;,
 2.22469;186.70559;-9.07996;,
 2.19898;186.70559;-12.02605;,
 -0.46737;186.70559;-12.00279;,
 -0.44166;186.70559;-9.05669;,
 -0.44166;169.65583;-9.05669;,
 -0.44818;169.65583;-9.80541;,
 2.21817;169.65583;-9.82868;,
 2.22469;169.65583;-9.07996;,
 -0.46737;183.76576;-12.00279;,
 -0.46737;186.70559;-12.00279;,
 2.19898;183.76576;-12.02605;,
 -0.44818;169.65583;-9.80541;,
 -0.46737;183.76576;-12.00279;,
 -0.44818;169.65583;-9.80541;,
 -0.44166;169.65583;-9.05669;,
 -0.44166;183.76576;-9.05669;,
 -0.44166;186.70559;-9.05669;,
 -0.46737;186.70559;-12.00279;,
 2.19898;183.76576;-12.02605;,
 2.19898;186.70559;-12.02605;,
 2.22469;186.70559;-9.07996;,
 2.22469;183.76576;-9.07996;,
 2.22469;169.65583;-9.07996;,
 2.21817;169.65583;-9.82868;,
 -0.44166;183.76576;-9.05669;,
 -0.44166;169.65583;-9.05669;,
 2.22469;183.76576;-9.07996;,
 -0.44166;186.70559;-9.05669;,
 -12.52751;272.50220;-0.75395;,
 -11.05461;272.50220;-0.75395;,
 -11.05461;186.69519;-0.75395;,
 -12.52751;186.69519;-0.75395;,
 -11.05461;272.50220;-0.75395;,
 -11.05461;272.50220;2.03497;,
 -11.05461;186.69519;2.03497;,
 -11.05461;186.69519;-0.75395;,
 -11.05461;272.50220;2.03497;,
 -12.52751;272.50220;2.03497;,
 -12.52751;186.69519;2.03497;,
 -11.05461;186.69519;2.03497;,
 -12.52751;272.50220;2.03497;,
 -12.52751;272.50220;-0.75395;,
 -12.52751;186.69519;-0.75395;,
 -12.52751;186.69519;2.03497;,
 -11.05461;272.50220;-0.75395;,
 -12.52751;272.50220;-0.75395;,
 -12.52751;186.69519;-0.75395;,
 -11.05461;186.69519;-0.75395;,
 2.36033;272.50220;-12.03187;,
 2.32610;272.50220;-10.69833;,
 2.32610;186.69519;-10.69833;,
 2.36033;186.69519;-12.03187;,
 2.32610;272.50220;-10.69833;,
 -0.46112;272.50220;-10.72628;,
 -0.46112;186.69519;-10.72628;,
 2.32610;186.69519;-10.69833;,
 -0.46112;272.50220;-10.72628;,
 -0.42688;272.50220;-12.05983;,
 -0.42688;186.69519;-12.05983;,
 -0.46112;186.69519;-10.72628;,
 -0.42688;272.50220;-12.05983;,
 2.36033;272.50220;-12.03187;,
 2.36033;186.69519;-12.03187;,
 -0.42688;186.69519;-12.05983;,
 2.32610;272.50220;-10.69833;,
 2.36033;272.50220;-12.03187;,
 2.36033;186.69519;-12.03187;,
 2.32610;186.69519;-10.69833;,
 -1.03154;272.43111;12.99647;,
 -0.91596;272.43111;11.53541;,
 -0.91596;186.62428;11.53541;,
 -1.03154;186.62428;12.99647;,
 -0.91596;272.43111;11.53541;,
 1.86434;272.43111;11.75313;,
 1.86434;186.62428;11.75313;,
 -0.91596;186.62428;11.53541;,
 1.86434;272.43111;11.75313;,
 1.74879;272.43111;13.21419;,
 1.74879;186.62428;13.21419;,
 1.86434;186.62428;11.75313;,
 1.74879;272.43111;13.21419;,
 -1.03154;272.43111;12.99647;,
 -1.03154;186.62428;12.99647;,
 1.74879;186.62428;13.21419;,
 -0.91596;272.43111;11.53541;,
 -1.03154;272.43111;12.99647;,
 -1.03154;186.62428;12.99647;,
 -0.91596;186.62428;11.53541;,
 11.97044;272.50220;-0.89745;,
 13.12404;272.50220;-0.89745;,
 13.12404;186.69519;-0.89745;,
 11.97044;186.69519;-0.89745;,
 13.12404;272.50220;-0.89745;,
 13.12404;272.50220;1.89143;,
 13.12404;186.69519;1.89143;,
 13.12404;186.69519;-0.89745;,
 13.12404;272.50220;1.89143;,
 11.97044;272.50220;1.89143;,
 11.97044;186.69519;1.89143;,
 13.12404;186.69519;1.89143;,
 11.97044;272.50220;1.89143;,
 11.97044;272.50220;-0.89745;,
 11.97044;186.69519;-0.89745;,
 11.97044;186.69519;1.89143;,
 13.12404;272.50220;-0.89745;,
 11.97044;272.50220;-0.89745;,
 11.97044;186.69519;-0.89745;,
 13.12404;186.69519;-0.89745;,
 -15.74385;274.65352;-16.01720;,
 15.74385;274.65352;-16.01720;,
 15.74385;272.61675;-16.01720;,
 -15.74385;272.61675;-16.01720;,
 15.74385;274.65352;-16.01720;,
 15.74385;274.65352;15.47055;,
 15.74385;272.61675;15.47055;,
 15.74385;272.61675;-16.01720;,
 15.74385;274.65352;15.47055;,
 -15.74385;274.65352;15.47055;,
 -15.74385;272.61675;15.47055;,
 15.74385;272.61675;15.47055;,
 -15.74385;274.65352;15.47055;,
 -15.74385;274.65352;-16.01720;,
 -15.74385;272.61675;-16.01720;,
 -15.74385;272.61675;15.47055;,
 15.74385;274.65352;-16.01720;,
 -15.74385;274.65352;-16.01720;,
 -15.74385;272.61675;-16.01720;,
 15.74385;272.61675;-16.01720;;
 
 176;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;8,11,12,13;,
 4;14,6,15,16;,
 4;14,16,17,1;,
 4;18,0,19,20;,
 4;18,20,21,7;,
 4;22,23,24,25;,
 4;22,25,26,27;,
 4;28,29,30,31;,
 4;32,33,34,35;,
 4;36,30,29,37;,
 4;38,39,40,41;,
 4;42,28,31,43;,
 3;44,35,34;,
 3;32,45,33;,
 3;46,41,40;,
 3;38,47,39;,
 4;37,48,49,36;,
 4;50,48,37,51;,
 4;52,49,48,53;,
 4;54,36,49,55;,
 3;50,53,48;,
 3;52,55,49;,
 4;53,56,57,52;,
 4;58,56,53,50;,
 4;59,57,56,60;,
 4;55,52,57,61;,
 3;58,60,56;,
 3;59,61,57;,
 4;62,63,64,65;,
 4;66,42,60,58;,
 4;67,68,47,38;,
 4;69,70,32,35;,
 4;38,41,71,67;,
 4;72,73,74,75;,
 4;76,77,78,79;,
 4;75,74,78,77;,
 4;80,72,75,81;,
 4;80,81,82,83;,
 4;84,85,86,87;,
 4;84,87,77,76;,
 4;81,87,86,82;,
 4;88,89,90,91;,
 4;88,92,93,94;,
 4;88,94,95,89;,
 4;91,90,96,97;,
 4;91,97,98,99;,
 4;100,101,102,103;,
 4;100,103,104,105;,
 4;106,107,108,109;,
 4;106,109,110,111;,
 4;103,102,112,113;,
 4;103,113,114,104;,
 4;109,108,115,116;,
 4;109,116,117,110;,
 4;113,112,118,119;,
 4;113,119,73,114;,
 4;116,115,79,120;,
 4;116,120,121,117;,
 4;122,123,124,125;,
 4;122,125,126,127;,
 4;128,129,130,131;,
 4;128,131,132,133;,
 4;125,131,130,126;,
 4;134,135,136,137;,
 4;134,137,138,139;,
 4;135,140,141,136;,
 4;142,143,144,145;,
 4;142,134,139,146;,
 4;142,146,147,143;,
 4;145,144,148,149;,
 4;145,149,140,135;,
 4;150,151,125,124;,
 4;152,150,124,132;,
 4;153,152,132,131;,
 4;151,153,131,125;,
 4;151,150,152,153;,
 4;154,155,88,91;,
 4;156,154,91,99;,
 4;157,156,99,92;,
 4;155,157,92,88;,
 4;155,154,156,157;,
 4;158,159,160,161;,
 4;162,163,164,165;,
 4;92,99,144,143;,
 4;93,92,143,147;,
 4;99,98,148,144;,
 4;102,101,147,146;,
 4;111,110,149,148;,
 4;112,102,146,139;,
 4;110,117,140,149;,
 4;118,112,139,138;,
 4;117,121,141,140;,
 4;124,123,138,137;,
 4;133,132,136,141;,
 4;132,124,137,136;,
 4;81,75,159,158;,
 4;75,77,160,159;,
 4;77,87,161,160;,
 4;87,81,158,161;,
 4;135,134,165,164;,
 4;142,145,163,162;,
 4;134,142,162,165;,
 4;145,135,164,163;,
 4;166,167,168,169;,
 4;170,171,172,173;,
 4;174,175,167,176;,
 4;174,176,172,177;,
 4;178,179,180,181;,
 4;178,181,182,183;,
 4;184,185,186,187;,
 4;184,187,188,189;,
 4;190,191,173,192;,
 4;190,192,166,193;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;202,203,195,204;,
 4;202,204,200,205;,
 4;206,207,208,209;,
 4;206,209,210,211;,
 4;212,213,214,215;,
 4;212,215,216,217;,
 4;218,219,201,220;,
 4;218,220,194,221;,
 4;222,223,224,225;,
 4;226,227,228,229;,
 4;230,231,223,232;,
 4;230,232,228,233;,
 4;234,235,236,237;,
 4;234,237,238,239;,
 4;240,241,242,243;,
 4;240,243,244,245;,
 4;246,247,229,248;,
 4;246,248,222,249;,
 4;250,251,252,253;,
 4;254,255,256,257;,
 4;258,259,251,260;,
 4;258,260,256,261;,
 4;262,263,264,265;,
 4;262,265,266,267;,
 4;268,269,270,271;,
 4;268,271,272,273;,
 4;274,275,257,276;,
 4;274,276,250,277;,
 4;278,279,280,281;,
 4;282,283,284,285;,
 4;286,287,288,289;,
 4;290,291,292,293;,
 4;290,283,294,295;,
 4;296,297,284,293;,
 4;298,299,300,301;,
 4;302,303,304,305;,
 4;306,307,308,309;,
 4;310,311,312,313;,
 4;310,303,314,315;,
 4;316,317,304,313;,
 4;318,319,320,321;,
 4;322,323,324,325;,
 4;326,327,328,329;,
 4;330,331,332,333;,
 4;330,323,334,335;,
 4;336,337,324,333;,
 4;338,339,340,341;,
 4;342,343,344,345;,
 4;346,347,348,349;,
 4;350,351,352,353;,
 4;350,343,354,355;,
 4;356,357,344,353;,
 4;358,359,360,361;,
 4;362,363,364,365;,
 4;366,367,368,369;,
 4;370,371,372,373;,
 4;370,363,374,375;,
 4;376,377,364,373;;
 
 MeshMaterialList {
  6;
  176;
  2,
  1,
  2,
  1,
  1,
  2,
  2,
  1,
  1,
  2,
  3,
  4,
  3,
  4,
  3,
  4,
  4,
  4,
  4,
  4,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  3,
  4,
  3,
  4,
  4,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  0,
  4,
  4,
  4,
  4,
  0,
  0,
  0,
  4,
  4,
  4,
  5,
  5,
  5,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4,
  4,
  5,
  5,
  4,
  4,
  4,
  4,
  5,
  5,
  4,
  4,
  4,
  4,
  5,
  5,
  4,
  5,
  4,
  4,
  5,
  5,
  4,
  4,
  4,
  5,
  5,
  5,
  4,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.800000;0.800000;0.800000;0.770000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.510000;0.510000;0.510000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\small\\stone_001.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\small\\sticker.png";
   }
  }
  Material {
   0.392000;0.370400;0.354400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   1.000000;1.000000;1.000000;1.000000;;
   0.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  123;
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.001761;0.036828;0.999320;,
  0.997386;0.071176;0.012457;,
  -0.994749;0.102342;-0.000408;,
  0.003519;0.073606;0.997281;,
  0.000000;0.000000;1.000000;,
  0.999837;0.006447;0.016874;,
  0.990731;0.135605;0.007988;,
  -0.984169;0.177235;0.000082;,
  -0.999639;0.026863;-0.000896;,
  0.001761;0.036902;-0.999317;,
  0.000000;0.000000;-1.000000;,
  0.003519;0.073754;-0.997270;,
  -0.367001;0.367016;-0.854757;,
  0.366999;0.367016;-0.854758;,
  0.367001;-0.366997;-0.854765;,
  -0.367003;-0.366997;-0.854764;,
  0.854758;0.367016;-0.366999;,
  0.854758;0.367016;0.366999;,
  0.854765;-0.366997;0.367002;,
  0.854765;-0.366997;-0.367001;,
  0.366999;0.367016;0.854758;,
  -0.367001;0.367016;0.854757;,
  -0.367003;-0.366997;0.854765;,
  0.367001;-0.366997;0.854765;,
  -0.854759;0.367015;0.366998;,
  -0.854759;0.367015;-0.366998;,
  -0.854766;-0.366996;-0.367000;,
  -0.854766;-0.366996;0.367000;,
  -0.366989;0.854774;0.366988;,
  0.366988;0.854775;0.366988;,
  0.366988;0.854775;-0.366988;,
  -0.366989;0.854774;-0.366987;,
  -0.367006;-0.854760;-0.367005;,
  0.367005;-0.854760;-0.367005;,
  0.367005;-0.854760;0.367005;,
  -0.367006;-0.854760;0.367005;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  -0.000001;1.000000;0.000000;,
  -0.000001;1.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.997018;-0.077172;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;,
  1.000000;0.000000;0.000000;,
  0.988089;-0.153883;-0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.996432;-0.083862;0.009470;,
  0.008013;-0.000000;0.999968;,
  -0.008013;0.000000;-0.999968;,
  -0.999955;0.000000;0.009503;,
  -0.985890;-0.167133;0.009370;,
  0.008009;-0.000000;0.999968;,
  0.008017;0.000000;0.999968;,
  -0.008017;0.000000;-0.999968;,
  -0.008009;0.000000;-0.999968;,
  0.999955;0.000000;-0.009488;,
  0.999955;0.000000;-0.009488;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.034790;-0.077171;0.996411;,
  0.999391;-0.000000;0.034900;,
  -0.999391;0.000000;-0.034903;,
  -0.034893;0.000000;0.999391;,
  -0.034479;-0.153882;0.987487;,
  0.999391;-0.000000;0.034904;,
  0.999391;0.000000;0.034896;,
  -0.999391;0.000000;-0.034896;,
  -0.999390;0.000001;-0.034911;,
  0.034893;0.000000;-0.999391;,
  0.034893;0.000000;-0.999391;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  -0.008698;-0.077172;-0.996980;,
  -0.999962;-0.000000;0.008722;,
  0.999962;0.000000;-0.008722;,
  -0.008723;0.000000;-0.999962;,
  -0.008621;-0.153883;-0.988051;,
  -0.999962;-0.000000;0.008717;,
  -0.999962;0.000000;0.008726;,
  0.999962;0.000000;-0.008727;,
  0.999962;0.000000;-0.008717;,
  0.008727;0.000000;0.999962;,
  0.008727;0.000000;0.999962;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.999671;0.000000;0.025660;,
  -0.010028;0.000000;0.999950;,
  -0.999671;0.000000;-0.025667;,
  0.010031;0.000000;-0.999950;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;0.000000;,
  -0.996886;0.000000;-0.078861;,
  0.078069;0.000000;-0.996948;,
  0.996887;0.000000;0.078840;,
  -0.078068;0.000000;0.996948;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;0.000000;1.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-1.000000;-0.000000;;
  176;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,5,5,2;,
  4;2,2,6,6;,
  4;3,7,7,3;,
  4;3,3,8,8;,
  4;4,9,9,4;,
  4;4,4,10,10;,
  4;11,12,12,11;,
  4;11,11,13,13;,
  4;14,15,16,17;,
  4;32,15,14,33;,
  4;21,16,15,18;,
  4;34,17,16,35;,
  4;27,14,17,28;,
  3;27,33,14;,
  3;32,18,15;,
  3;21,35,16;,
  3;34,28,17;,
  4;18,19,20,21;,
  4;31,19,18,32;,
  4;25,20,19,22;,
  4;35,21,20,36;,
  3;31,22,19;,
  3;25,36,20;,
  4;22,23,24,25;,
  4;30,23,22,31;,
  4;29,24,23,26;,
  4;36,25,24,37;,
  3;30,26,23;,
  3;29,37,24;,
  4;26,27,28,29;,
  4;33,27,26,30;,
  4;37,29,28,34;,
  4;30,31,32,33;,
  4;34,35,36,37;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;38,38,38,38;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;40,40,40,40;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;40,40,40,40;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;43,43,43,43;,
  4;41,41,41,41;,
  4;38,38,38,38;,
  4;44,44,44,44;,
  4;43,43,43,43;,
  4;42,42,42,42;,
  4;43,43,43,43;,
  4;45,45,45,45;,
  4;38,38,38,38;,
  4;41,41,41,41;,
  4;39,39,39,39;,
  4;38,38,38,38;,
  4;43,43,43,43;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;40,40,40,40;,
  4;41,41,41,41;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;42,42,42,42;,
  4;41,41,41,41;,
  4;39,39,39,39;,
  4;40,40,40,40;,
  4;42,42,42,42;,
  4;39,39,39,39;,
  4;42,42,42,42;,
  4;41,41,41,41;,
  4;40,40,40,40;,
  4;46,46,46,46;,
  4;47,47,47,47;,
  4;48,51,51,48;,
  4;48,48,52,52;,
  4;49,49,49,49;,
  4;49,49,49,49;,
  4;50,50,50,50;,
  4;50,50,50,50;,
  4;53,53,53,53;,
  4;53,53,53,53;,
  4;54,54,54,54;,
  4;55,55,55,55;,
  4;56,59,59,56;,
  4;56,56,60,60;,
  4;57,61,61,57;,
  4;57,57,62,62;,
  4;58,63,63,58;,
  4;58,58,64,64;,
  4;65,65,65,65;,
  4;65,65,66,66;,
  4;67,67,67,67;,
  4;68,68,68,68;,
  4;69,72,72,69;,
  4;69,69,73,73;,
  4;70,74,74,70;,
  4;70,70,75,75;,
  4;71,76,76,71;,
  4;71,71,77,77;,
  4;78,78,78,78;,
  4;78,78,79,79;,
  4;80,80,80,80;,
  4;81,81,81,81;,
  4;82,85,85,82;,
  4;82,82,86,86;,
  4;83,87,87,83;,
  4;83,83,88,88;,
  4;84,89,89,84;,
  4;84,84,90,90;,
  4;91,92,92,91;,
  4;91,91,91,91;,
  4;93,93,93,93;,
  4;94,94,94,94;,
  4;95,95,95,95;,
  4;96,96,96,96;,
  4;97,97,97,97;,
  4;98,98,98,98;,
  4;99,99,99,99;,
  4;100,100,100,100;,
  4;101,101,101,101;,
  4;102,102,102,102;,
  4;103,103,103,103;,
  4;104,104,104,104;,
  4;105,105,105,105;,
  4;106,106,106,106;,
  4;107,107,107,107;,
  4;108,108,108,108;,
  4;109,109,109,109;,
  4;110,110,110,110;,
  4;111,111,111,111;,
  4;112,112,112,112;,
  4;113,113,113,113;,
  4;114,114,114,114;,
  4;115,115,115,115;,
  4;116,116,116,116;,
  4;117,117,117,117;,
  4;118,118,118,118;,
  4;119,119,119,119;,
  4;120,120,120,120;,
  4;121,121,121,121;,
  4;122,122,122,122;;
 }
 MeshTextureCoords {
  378;
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.908440;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.908440;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.908440;,
  0.000000;1.000000;,
  0.000000;0.908440;,
  0.000000;0.000000;,
  0.000000;0.908440;,
  1.000000;0.000000;,
  1.000000;0.908440;,
  1.000000;1.000000;,
  1.000000;0.908440;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.908440;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.087430;0.009450;,
  0.912580;0.009450;,
  0.912580;0.990550;,
  0.087430;0.990550;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.990550;,
  1.000000;0.009450;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000010;0.009450;,
  0.000010;0.990550;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.009450;,
  1.000000;0.990550;,
  0.912580;-0.000000;,
  0.912580;-0.000000;,
  0.912580;0.990550;,
  0.912580;0.009450;,
  0.912580;1.000000;,
  0.912580;1.000000;,
  0.087430;0.009450;,
  0.087430;0.990550;,
  0.087430;-0.000000;,
  0.000010;0.990550;,
  0.000010;0.009450;,
  0.087430;1.000000;,
  0.000000;0.009450;,
  0.000000;0.009450;,
  0.000000;0.990550;,
  0.000000;0.990550;,
  0.087430;-0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.899890;0.000000;,
  1.000000;0.000000;,
  1.000000;0.032050;,
  0.899890;0.032050;,
  0.899890;1.000000;,
  0.899890;0.965180;,
  1.000000;0.965180;,
  1.000000;1.000000;,
  0.116630;0.000000;,
  0.116630;0.032050;,
  0.000000;0.032050;,
  0.000000;0.000000;,
  0.116630;1.000000;,
  0.000000;1.000000;,
  0.000000;0.965180;,
  0.116630;0.965180;,
  0.855220;0.032050;,
  1.000000;0.032050;,
  1.000000;0.965180;,
  0.855220;0.965180;,
  0.123260;0.032050;,
  0.123260;0.000000;,
  0.855220;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.855220;1.000000;,
  0.123260;1.000000;,
  0.123260;0.965180;,
  0.855220;1.000000;,
  0.123260;1.000000;,
  0.123260;0.883370;,
  0.855220;0.883370;,
  1.000000;0.883370;,
  1.000000;1.000000;,
  0.855220;0.000000;,
  1.000000;0.000000;,
  1.000000;0.116630;,
  0.855220;0.116630;,
  0.123260;0.116630;,
  0.123260;0.000000;,
  0.123260;0.100110;,
  0.855220;0.100110;,
  1.000000;0.100110;,
  1.000000;0.899890;,
  0.855220;0.899890;,
  0.123260;0.899890;,
  0.123260;0.000000;,
  0.855220;0.000000;,
  0.855220;1.000000;,
  0.123260;1.000000;,
  0.144780;0.000000;,
  0.876740;0.000000;,
  0.876740;0.032050;,
  0.144780;0.032050;,
  0.000000;0.032050;,
  0.000000;0.000000;,
  0.144780;1.000000;,
  0.000000;1.000000;,
  0.000000;0.965180;,
  0.144780;0.965180;,
  0.876740;0.965180;,
  0.876740;1.000000;,
  0.100110;0.032050;,
  0.100110;0.965180;,
  0.000000;0.965180;,
  0.000000;0.032050;,
  0.000000;0.000000;,
  0.100110;0.000000;,
  0.100110;1.000000;,
  0.000000;1.000000;,
  0.883370;0.032050;,
  1.000000;0.032050;,
  1.000000;0.965180;,
  0.883370;0.965180;,
  0.883370;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.883370;1.000000;,
  0.876740;0.032050;,
  0.144780;0.032050;,
  0.876740;0.965180;,
  0.144780;0.965180;,
  0.855220;0.965180;,
  0.855220;0.032050;,
  0.123260;0.965180;,
  0.123260;0.032050;,
  0.116630;0.032050;,
  0.899890;0.032050;,
  0.899890;0.965180;,
  0.116630;0.965180;,
  0.883370;0.032050;,
  0.883370;0.965180;,
  0.100110;0.965180;,
  0.100110;0.032050;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.043110;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;0.043110;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.043110;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;0.043110;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.043110;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;0.043110;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.043110;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.043110;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;0.043110;,
  1.000000;1.000000;,
  0.000000;0.043110;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  1.000000;1.000000;,
  0.000000;1.000000;,
  0.000000;0.000000;,
  1.000000;0.000000;;
 }
}