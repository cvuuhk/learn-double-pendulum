#include "raylib.h"
#include <math.h>

const int windowWidth = 2560, windowHeight = 1440;
const Vector2 fixed = {.x = windowWidth / 2.0, .y = 0};
const float G = 10;
const float R = 20;
const float L1 = 400, L2 = 200;
const float M1 = 3, M2 = 0.5;

float theta1, theta2, theta1_d, theta2_d;

void init(void) {
  theta1 = GetRandomValue(-90, 90) * DEG2RAD;
  theta2 = GetRandomValue(-90, 90) * DEG2RAD;

  theta1_d = 0;
  theta2_d = 0;
}

void drawPendulum(void) {
  Vector2 p1 = {.x = fixed.x + L1 * sinf(theta1), .y = fixed.y + L1 * cosf(theta1)};
  Vector2 p2 = {.x = p1.x + L2 * sinf(theta2), .y = p1.y + L2 * cosf(theta2)};
  DrawLineV(fixed, p1, WHITE);
  DrawLineV(p1, p2, WHITE);

  DrawCircle(p1.x, p1.y, R, RED);
  DrawCircle(p2.x, p2.y, R, RED);
}

void refresh(float dt) {
  //                    −g(2m₁+m₂)sin θ₁ − m₂g sin(θ₁−2θ₂) − 2 sin(θ₁−θ₂)·m₂(θ̇₂²L₂ + θ̇₁²L₁ cos(θ₁−θ₂))
  // 上摆角加速度 θ̈₁ = ────────────────────────────────────────────────────────────────────────────────
  //                       L₁[ 2m₁+m₂ − m₂ cos(2θ₁−2θ₂) ]
  //
  //                   2 sin(θ₁−θ₂)[ θ̇₁²L₁(m₁+m₂) + g(m₁+m₂)cos θ₁ + θ̇₂²L₂m₂ cos(θ₁−θ₂) ]
  // 下摆角加速度 θ̈₂ = ──────────────────────────────────────────────────────────────────
  //                     L₂[ 2m₁+m₂ − m₂ cos(2θ₁−2θ₂) ]
  //
  float phi1_dd=(-G*(2*M1+M2)*sinf(theta1)-M2*G*sinf(theta1-2*theta2)-2*sinf(theta1-theta2)*M2*(theta2_d*theta2_d*L2+theta1_d*theta1_d*L1*cosf(theta1-theta2)))/(L1*(2*M1+M2-M2*cosf(2*theta1-2*theta2)));
  float phi2_dd=(2*sinf(theta1-theta2)*(theta1_d*theta1_d*L1*(M1+M2)+G*(M1+M2)*cosf(theta1)+theta2_d*theta2_d*L2*M2*cosf(theta1-theta2)))/(L2*(2*M1+M2-M2*cosf(2*theta1-2*theta2)));

  theta1_d += phi1_dd * dt;
  theta2_d += phi2_dd * dt;

  theta1 += theta1_d * dt;
  theta2 += theta2_d * dt;
}

int main(void) {
  InitWindow(windowWidth, windowHeight, "双摆");
  SetTargetFPS(144);

  init();
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);

    refresh(GetFrameTime() * 10); // 动画加速
    drawPendulum();
    EndDrawing();
  }

}
