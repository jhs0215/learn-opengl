# learn-opengl
- [learnopengl](https://learnopengl.com/)

# Hello Triangle

## Normalized Device Coordinates (NDC) : 정규화 좌표계
- 정점 좌표가 정점 셰이더에서 처리되면 정규화 좌표 x, y 그리고 z 에서 -1.0 ~ 1.0 로 표현된 작은 공간입니다. 이범위를 벗어나는 좌표는 절단되어 화면에 표시되지 않습니다. 
- 화면 좌표는 뷰포트(glViewport) 변환을 통해 정규화 좌표계로 치환됩니다.

## Vertex Buffer Object (VBO) : 정점 버퍼 오브젝트 
- 정점 버퍼 오브젝트 GPU 의 메모리에 많은 수의 정점을 저장할 수 있습니다. 이러한 버퍼 객체를 사용하는 이점은 한 번에 한 정점 씩 데이터를 보낼 필요없이 대량의 데이터를 한 번에 그래픽 카드로 보내고 충분한 메모리가 남아있는 경우 그대로 유지할 수 있다는 것입니다. CPU에서 그래픽 카드로 데이터를 보내는 것은 비교적 느리기 때문에 가능한 한 많은 데이터를 한 번에 보내려고 할 수 있습니다. 데이터가 그래픽 카드의 메모리에 있으면 정점 셰이더가 정점에 거의 즉각적으로 액세스하여 매우 빠릅니다.

```cpp
unsigned int vbo
glGenBuffers(1, &vbo); // 해당 버퍼에 해당하는 고유 ID 할당
glBindBuffer(GL_ARRAY_BUFFER, VBO); // GL_ARRAY_BUFFER 타겟에 바인딩 
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // 바인딩 된 버퍼의 메모리에 값 복사 
```
## 그래픽 카드가 주어진 데이터를 관리하는 방법
- GL_STREAM_DRAW : 데이터가 한 번만 설정되고 GPU에서 최대 몇 번만 사용됩니다.
- GL_STATIC_DRAW : 데이터가 한 번만 설정되고 여러 번 사용됩니다.
- GL_DYNAMIC_DRAW : 데이터가 많이 변경되고 여러 번 사용됩니다.

## Shader
```glsl
// Vertex Shader

#version 330 core
layout (local = 0) in vec3 aPos;

void main()
{
    # gl_position : predefined variable
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
```

```glsl
// Fragment Shader

#version 330 core
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f); // Red Color 
}
```

## Shader Compile : 셰이더 컴파일
```cpp
const char* vertexShaderSource; // Load Shader Source (*.glsl)

unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); // Vertex 세이더 생성 
glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); 
glCompileShader(vertexShader); // 셰이더 컴파일

// 컴파일 성공여부 확인
int success;
char infoLog[512];
glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
if(!success)
{
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    std::cout << "Compile failed" << infoLog << std::endl;
})

// Fragment 셰이더는 Shader type(GL_FRAGMENT_SHADER) 을 제외하고 Vertex Shader 와 동일하게 컴파일을 진행한다.
```

## Shader Program : 셰이더 프로그램
- 셰이더 프로그램 개체는 결합 된 여러 셰이더의 최종 링크 버전입니다. 최근에 컴파일된 셰이더를 사용하려면 링크된 셰이더 프로그램 개체를 추가 한 다음 개체를 렌더링 할 때에 셰이더 프로그램을 활성화 한다.

```cpp
unsigned int shaderProgram = glCreateProgram();
glAttachShader(shaderProgram, vertexShader);
glAttachShader(shaderProgram, fragmentShader);
glLinkProgram(shaderProgram);

// 셰이더 프로그램 연결이 실패했는지 확인
int success;
char infoLog[512];
glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
if(!success)
{
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Link failed" << infoLog << std::endl;
}

// 프로그램 활성화 
glUseProgram(shaderProgram);

// Link 가 완료된 셰이더 객체는 삭제 
glDeleteShader(vertexShader);
glDeleteShader(fragmentShader);
```

## Linking Vertex Attribute : 정점 속성 연결
- 정점 셰이더를 사용하면 정점 속성의 형태로 원하는 입력을 지정할 수 있으며, 이는 유연성을 크게 허용하지만 입력 데이터의 어떤 부분이 정점 셰이더의 어떤 정점 속성으로 이동하는지 수동으로 지정해야 함을 의미합니다. 이는 OpenGL이 렌더링 전에 정정 데이터를 해석하는 방법을 지정해야 함을 의미합니다.
- 정점 속성은 현재 GL_ARRAY_BUFFER에 바인딩 된 VBO 에 의해 결정됩니다.
```cpp
// 0. copy our vertices array in a buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, vbo); 
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 1. then set the vertex attributes pointers
glVertexAttributePointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // 첫 번째 매개 변수는 정점 속성을 지정합니다.
glEnableVertexAttribute(0); // 정점 속성을 활성화
// 2. use our shader program when we want to render an object
glUseProgram(shaderProgram);
// 3. now draw the object
```

## Element Buffer Object (EBO) : 요소 버퍼 객체

```cpp
unsigned int ebo;
glGenBuffers(1, &ebo); // 요소 버퍼 객체 생성

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
```

## Vertex Array Object (VAO) : 정점 배열 객체
- vertex array object 
- vertex buffer object
- element buffer object
- vertex attribute 
- draw 
    - bind shader program
    - bind vertex array object 
    - draw element 
    - unbind all
```cpp
unsigned int vao;
glGenVertexArrays(1, &vao);
// 1. bind vertex array object
glBindVertexArray(vao);
// 2. copy our vertics array in a buffer for OpenGL to use
glBindBuffer(GL_ARRAY_BUFFER, vbo);
glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), GL_STATIC_DRAW);
// 3. copy our index array in a element buffer for OpenGL to use
glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 4. then set our vertex attributes pointers
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); 
glEnableVertexAttribAray(0);
// 5. draw the object
glUseProgram(shaderProgram);
glBindVertexArray(vao);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
```

# 셰이더
```cpp
// 정점 속성의 최대 개수 구하기 (최소 16개는 가능하다.)
int nrAttributes;
glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
std::cout << "Maximum nr of vertex attributes supported : " << nrAttributes << std::endl;
```
- 정점 셰이더는 정점 데이터가 구성되는 방식을 정의하기 위해 위치 메타 데이터로 입력 변수를 지정하여 CPU에서 정점 속성을 구성 할 수 있습니다. 정점 셰이더에는 입력에 대한 추가 레이아웃 사양이 필요하므로 정점 데이터와 연결할 수 있습니다.
- layout (location = 0) 지정자를 생략하고 glGetAttribLocation 을 이용하여 속성 위치를 쿼리 할 수도 있습니다. 

## in, out
```glsl 
// Vertex Shader 

#version 330 core
layout (location = 0) in vec3 aPos; // attribute position 0
out vec4 vertexColor; // specify a color output to the fragment shader

void main()
{
    gl_Position = vec4(aPos, 1.0);
    vertexColor = vec4(0.5f, 0.0f, 0.0f, 1.0f); // dark-red
}
```

```glsl
// Fragment Shader

#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // the input variable from the vertex shader 

void main()
{
    FragColor = vertexColor;
}
```

## uniform
- cpu의 애플리케이션에서 gpu 셰이더로 데이터를 전달하는 또 다른 방법이다. 
- uniform 은 global 변수이며, 모든 셰이더에서 엑세스 할 수 있음
- uniform 값을 설정하는 것과 관계없이 uniform은 재설정 되거나 업데이트 될 때까지 값을 유지한다.

```glsl
// Fragment Shader

#version 330 core
out vec4 FragColor;

uniform vec4 ourColor; // we set this variable in the OpenGL code

void main()
{
    FragColor = ourColor;
}
```
```cpp
glClearColor(0.2f, 0.0f, 0.0f, 1.0f)
glClear(GL_COLOR_BUFFER_BIT);

float timeValue = glfwGetTime();
float greenValue = (sin(timeValue) / 2.0f) + 0.5f; // 0 ~ 1
int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor"); // layout location 검색

// 현재 활성화 된 셰이더 프로그램에서 유니폼을 설정
glUseProgram(shaderProgram);
glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

glBindVertexArray(vao);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
glBindVertexArray(0);

glfwSwapBuffers(window);
```

# Texture : 텍스처

## Texture Wrapping : 텍스처 래핑
- 텍스처 좌표는 일반적으로 (0, 0) ~ (1, 1) 이지만 범위를 벗어난 좌표를 지정하면 기본 동작은 텍스처 이미지를 반복하는 것이다. 
    - GL_REPEAT : 텍스처 이미지를 반복 (기본값)
    - GL_MIRRORED_REPEAT : GL_REPEAT 와 같지만 이미지를 미러링한다.
    - GL_CLAMP_TO_EDGE : 0 ~ 1 이를 벗어나면 가장자리의 고정된 패턴이 늘어난다. 
    - GL_CLAMP_TO_BORDER : 범위를 벗어난 좌표에 사용자가 지정한 테두리 색상이 지정된다.

```cpp
// Wrapping 설정
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
// Border 설정
float borderColor[] = {1.f, 1.f, 1.f, 1.f};
glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
```

## Texture Filtering : 텍스처 필터링
- GL_NEAREST : 중심이 텍스처 좌표에 가장 가까운 텍셀을 선택합니다. (기본값)
- GL_LINEAR : 텍스처 좌표의 인접한 텍셀에서 보간 된 값을 가져온다. 텍스처 좌표에서 텍셀 중심까지의 거리가 짧을수록 해당 텍셀의 색상이 샘플링 된 색상에 더 많이 기여한다. 

```cpp
// 다운 스케일은 Nearst Texture, 업 스케일은 Linear Texture
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINER);
```

## 밉맵
- GL_NEAREST_MIPMAP_NEAREST : 픽셀 크기와 일치하는 가장 가까운 밉맵을 선택하고, 가장 가까운 텍스처를 사용한다. 
- GL_LINEAR_MIPMAP_NEAREST : 픽셀 크기와 일치하는 가장 가까운 밉맵을 선택하고, 선형 보간을 사용하여 샘플링하여 사용한다.
- GL_NEAREST_MIPMAP_LINER : 픽셀 크기에 가장 인접한 밉맵을 선택하여 선형 보간하고, 가장 가까운 텍스처를 사용한다. 
- GL_LINEAR_MIPMAP_LINER : 픽셀 크기에 가장 인접한 밉맵을 선택하여 선형 보간하고, 선형 보간을 사용하여 샘플링하여 사용한다.

```cpp
// 일반적으로 밉맵 필터링 옵션 중 하나를 확대 필터로 설정하는데, 밉맵은 주로 텍스처가 축소 될 때 사용되기 때문에 효과가 없다. 
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINER);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINER);
```

## 텍스처 생성
```cpp
// Texture 생성
unsigned int texture;
glGenTextures(1, &texture);
glBindTexture(GL_TEXTURE_2D, texture);

glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

// Texture data
int width, heigh, nrChannels;
unsigned char * data = stbi_load("Resource\\Image\\container.jpg", &width, &heigh, &nrChannels, 0);
if(data)
{
    // Texture Data 할당
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);    
}
stbi_image_free(data);

// vertex buffer 에 texture coordinate 추가
// vertex attribute 에 texture 속성 추가
// shader 수정


// sampler 에 속한 텍스처 단위는 한번만 설정하면 되므로 렌더링 루프에 들어가기 전에 작업을 수행
ourShader.use();
glActiveTexture(GL_TEXTURE0); // 사용할 텍스처 유닛을 전달
glUniform1i(glGetUniformLocation(ourShader.ID, "texture1"), 0);
glActiveTexture(GL_TEXTURE0 + 1);
ourShader.setInt("texture2", 1); 

// draw 
glBindTexture(GL_TEXTURE_2D, texture);
glBindVertexArray(vao);
glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

```
 
 # Depth Test : 깊이 테스트 
 - 깊이 테스트는 프래그먼트 셰이더가 실행 된 후 화면 공간에서 수행됩니다.
 
 ```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Depth buffer 초기화
glEnable(GL_DEPTH_TEST); // Depth test 활성화
glDepthFunc(GL_LEQUAL); // Depth 가 작거나 같으면 통과
glDepthMask(GL_FALSE); // Depth buffer 비활성화 (Read Only Mode)
 ```

 # Stencil Buffer : 스텐실 버퍼
 - 스텐실 버퍼에 쓰기를 활성화 합니다. 
 - 스텐실 버퍼의 내용을 업데이트하여 개체를 렌더링합니다. 
 - 스텐실 버퍼에 쓰기를 비활성화 합니다.
 - 개체를 렌더링합니다. 이번에는 스텐실 버퍼의 내용에 따라 특정 프레그먼트를 삭제합니다. 

```cpp
glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
glEnable(GL_STENCIL_TEST);
glStencilMask(0xFF); // each bit is written to the stencil buffer as is
glStencilMask(0x00); // each bit ends up as 0 in the stencil buffer (disabling writes)
```

# Shadow 
## Ambient Occlusion (AO)
- 렌더링 과정 중에 셰이딩의 한 방식이며, 각각의 표면이 광원에 얼마나 노출되어 있는지를 계산하여 그림자를 더해주는 기술이다. (차폐영역을 더 어둡게 만드는 기술)
- 일반적으로 광원과 물체를 던져놓고 __지역조명__ 방식으로 처리를 시행하면 그림자는 있지만 실제와 미묘하게 다른데 AO는 이를 보정하고 실제에 더 가깝게 보이도록 만든다. 
- __지역조명이란__ 색을 입이고 후처리로 음영을 입힘

## AO 의 종류
### ssao (screen space ambient occlusion)
- 가장 많이 사용되는 ao. 현재 렌더링되어 있는 영역만을 screen space 로 인식하여 해당 영역만을 ambient occlusion 을 적용함으로써 일정 수준 최적화된 그래픽을 제공한다. 

### HBAO+ (horizon based ambient occlusion +)
- NVidia 가 제공하는 기술. 최근들어 쓰이는 차세대 ao. ssao 보다 좀더 높은 연산 능력을 필요로 하지만 그림자의 정확성을 좀더 높인다. 

## shadow mapping 
- 그림자는 acclusion 에 의해 빛이 없는 영역이다. 

## bias
- depth map 과 scene 의 해상도 오차로 인해서 shadow acne 이 발생할 수 있는데, 이때 작은 offset 값을 이용하여 surface depth 를 표면보다 높게 할당하면 그림자가 올바로 출력될 수 있다. 여기서 작은 offset 값을 bias 라고 한다. 
- shadow acne 은 lightDir 과 normal 의 angle 이 커질수록 잘 발생된다. 그렇기 때문에 각도에 따라서 bias 값을 조절하여 퀄리티가 놓은 shadow 를 생성할 수 있다. 
```glsl
float fBias = max(0.005, 0.005 * (1.0 - dot(normal, lightDir)));
float fShadow = fCurrentDepth - fBias > fClosestDepth ? 1.0f : 0.0f;
```

## PCF (percentage-closer filtering)
- 주변 pixel 에 해당하는 값을 이용하여 shadow 의 값을 조절하는 기법을 의미한다.