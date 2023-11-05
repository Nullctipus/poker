#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform int frames;
uniform sampler2D texture0;
uniform vec4 colDiffuse;

vec4 _EffectColor = vec4(1.0, 1.0, 1.0, 1.0);
#define _StartLength 0.1
#define _IncLength 0.001
#define _EndLength 1.5
#define _TimeScale 240.0
#define _Scale 4.0
#define _HorizontalWrapCount 22.0
#define _HorizontalLoopCount 1.52
#define _HorizontalSpeed 2.0
#define _VerticalWrapCount 11.0
#define _VerticalLoopCount 4.0
#define _VerticalSpeed 2.5
#define _ThicknessMultiplier 0.03
#define _ThicknessOffset 0.5
#define _ThicknessDelta 0.4
#define _ThicknessFreq 24.0

// NOTE: Add here your custom variables
vec2 resolution = vec2(800.0, 450.0);
vec3 Lerp(vec3 a, vec3 b, float t) { return a + t * (b - a); }
const float PI = 3.14159;

void main() {
  vec4 texelColor = texture2D(texture0, fragTexCoord);
  float x = 1.0 / resolution.x;
  float y = 1.0 / resolution.y;
  vec2 uv = (fragTexCoord - 0.5) * 2.0;
  float time = float(frames) / _TimeScale;
  float v = 0.0;
  for (float i = _StartLength; i < _EndLength; i += _IncLength) {
    v += float(
        length(uv + vec2(i * cos(i * _HorizontalWrapCount * sin(time)) * i *
                             sin(_HorizontalLoopCount *
                                 cos(time * _HorizontalSpeed) * i * cos(i)),
                         sin(i * _VerticalWrapCount * sin(time)) *
                             sin(_VerticalLoopCount *
                                     sin(time * _VerticalSpeed) * i * i +
                                 sin(cos(time) + time)))) <
        _ThicknessMultiplier *
            (_ThicknessOffset + _ThicknessDelta * sin(i * _ThicknessFreq)));
  }

  // return fixed4(Lerp(col.rgb,hueShift(_EffectColor.rgb, frac(_Time.y *
  // _EffectHue)),min(v,1.0)),col.a + min(v,1.0) * (_EffectColor.a - col.a));
  gl_FragColor =
      vec4(min(v, 1.0) * _EffectColor.rgb, _EffectColor.a * min(v, 1.0));
}