#include "IncludeOgl.h"
#include "GlfwMultipleLights.h"

// positions all containers
glm::vec3 GlfwMultipleLights::cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};
// positions of the point lights
glm::vec3 GlfwMultipleLights::pointLightPositions[] = {
	glm::vec3(0.7f,  0.2f,  2.0f),
	glm::vec3(2.3f, -3.3f, -4.0f),
	glm::vec3(-4.0f,  2.0f, -12.0f),
	glm::vec3(0.0f,  0.0f, -3.0f)
};

GlfwMultipleLights::GlfwMultipleLights()
	: m_lightingShader("resources/shaders/6.multiple_lights.vs.glsl", "resources/shaders/6.multiple_lights.fs.glsl")
	, m_lightCubeShader("resources/shaders/6.light_cube.vs.glsl", "resources/shaders/6.light_cube.fs.glsl")
{
}

GlfwMultipleLights::~GlfwMultipleLights()
{
}

void GlfwMultipleLights::build()
{
	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};
	//// positions all containers
	//glm::vec3 cubePositions[] = {
	//	glm::vec3(0.0f,  0.0f,  0.0f),
	//	glm::vec3(2.0f,  5.0f, -15.0f),
	//	glm::vec3(-1.5f, -2.2f, -2.5f),
	//	glm::vec3(-3.8f, -2.0f, -12.3f),
	//	glm::vec3(2.4f, -0.4f, -3.5f),
	//	glm::vec3(-1.7f,  3.0f, -7.5f),
	//	glm::vec3(1.3f, -2.0f, -2.5f),
	//	glm::vec3(1.5f,  2.0f, -2.5f),
	//	glm::vec3(1.5f,  0.2f, -1.5f),
	//	glm::vec3(-1.3f,  1.0f, -1.5f)
	//};
	//// positions of the point lights
	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(0.7f,  0.2f,  2.0f),
	//	glm::vec3(2.3f, -3.3f, -4.0f),
	//	glm::vec3(-4.0f,  2.0f, -12.0f),
	//	glm::vec3(0.0f,  0.0f, -3.0f)
	//};
	// first, configure the cube's VAO (and VBO)
	//unsigned int VBO, cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	//unsigned int lightCubeVAO;
	glGenVertexArrays(1, &lightCubeVAO);
	glBindVertexArray(lightCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// load textures (we now use a utility function to keep the code more organized)
	// -----------------------------------------------------------------------------
	diffuseMap = loadTexture("resources/textures/container2.png");
	specularMap = loadTexture("resources/textures/container2_specular.png");

	// shader configuration
	// --------------------
	m_lightingShader.use();
	m_lightingShader.setInt("material.diffuse", 0);
	m_lightingShader.setInt("material.specular", 1);
}

void GlfwMultipleLights::draw()
{
	// render
		// ------
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// be sure to activate shader when setting uniforms/drawing objects
	m_lightingShader.use();
	m_lightingShader.setVec3("viewPos", camera.Position);
	m_lightingShader.setFloat("material.shininess", 32.0f);

	/*
	   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
	   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
	   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
	   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
	*/
	// directional light
	m_lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
	m_lightingShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
	m_lightingShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
	m_lightingShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
	// point light 1
	m_lightingShader.setVec3("pointLights[0].position", pointLightPositions[0]);
	m_lightingShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
	m_lightingShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
	m_lightingShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setFloat("pointLights[0].constant", 1.0f);
	m_lightingShader.setFloat("pointLights[0].linear", 0.09f);
	m_lightingShader.setFloat("pointLights[0].quadratic", 0.032f);
	// point light 2
	m_lightingShader.setVec3("pointLights[1].position", pointLightPositions[1]);
	m_lightingShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
	m_lightingShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
	m_lightingShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setFloat("pointLights[1].constant", 1.0f);
	m_lightingShader.setFloat("pointLights[1].linear", 0.09f);
	m_lightingShader.setFloat("pointLights[1].quadratic", 0.032f);
	// point light 3
	m_lightingShader.setVec3("pointLights[2].position", pointLightPositions[2]);
	m_lightingShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
	m_lightingShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
	m_lightingShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setFloat("pointLights[2].constant", 1.0f);
	m_lightingShader.setFloat("pointLights[2].linear", 0.09f);
	m_lightingShader.setFloat("pointLights[2].quadratic", 0.032f);
	// point light 4
	m_lightingShader.setVec3("pointLights[3].position", pointLightPositions[3]);
	m_lightingShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
	m_lightingShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
	m_lightingShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setFloat("pointLights[3].constant", 1.0f);
	m_lightingShader.setFloat("pointLights[3].linear", 0.09f);
	m_lightingShader.setFloat("pointLights[3].quadratic", 0.032f);
	// spotLight
	m_lightingShader.setVec3("spotLight.position", camera.Position);
	m_lightingShader.setVec3("spotLight.direction", camera.Front);
	m_lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
	m_lightingShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
	m_lightingShader.setFloat("spotLight.constant", 1.0f);
	m_lightingShader.setFloat("spotLight.linear", 0.09f);
	m_lightingShader.setFloat("spotLight.quadratic", 0.032f);
	m_lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
	m_lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

	// view/projection transformations
	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	m_lightingShader.setMat4("projection", projection);
	m_lightingShader.setMat4("view", view);

	// world transformation
	glm::mat4 model = glm::mat4(1.0f);
	m_lightingShader.setMat4("model", model);

	// bind diffuse map
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	// bind specular map
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, specularMap);

	// render containers
	glBindVertexArray(cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		// calculate the model matrix for each object and pass it to shader before drawing
		model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_lightingShader.setMat4("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	// also draw the lamp object(s)
	m_lightCubeShader.use();
	m_lightCubeShader.setMat4("projection", projection);
	m_lightCubeShader.setMat4("view", view);

	// we now draw as many light bulbs as we have point lights.
	glBindVertexArray(lightCubeVAO);
	for (unsigned int i = 0; i < 4; i++)
	{
		model = glm::mat4(1.0f);
		model = glm::translate(model, pointLightPositions[i]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		m_lightCubeShader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void GlfwMultipleLights::release()
{
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightCubeVAO);
	glDeleteBuffers(1, &VBO);
}
