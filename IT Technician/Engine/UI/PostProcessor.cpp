#include "PostProcessor.h"

#include <glad/glad.h>

#include "../ResourceManager.h"

PostProcessor::PostProcessor(int width, int height) : _width(width), _height(height) {
    // initialize renderbuffer/framebuffer object
    glGenFramebuffers(1, &this->_MSFBO);
    glGenFramebuffers(1, &this->_FBO);
    glGenRenderbuffers(1, &this->_RBO);

    // initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, this->_MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->_RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB, width, height); // allocate storage for render buffer object
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->_RBO); // attach MS render buffer object to framebuffer

    // also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, this->_FBO);
    this->_texture.Load(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->_texture.ID, 0); // attach texture to framebuffer as its color attachment

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // initialize render data and uniforms
    this->_initRenderData();
    //this->PostProcessingShader.SetInteger("scene", 0, true);
    //float offset = 1.0f / 300.0f;
    //float offsets[9][2] = {
    //    { -offset,  offset  },  // top-left
    //    {  0.0f,    offset  },  // top-center
    //    {  offset,  offset  },  // top-right
    //    { -offset,  0.0f    },  // center-left
    //    {  0.0f,    0.0f    },  // center-center
    //    {  offset,  0.0f    },  // center - right
    //    { -offset, -offset  },  // bottom-left
    //    {  0.0f,   -offset  },  // bottom-center
    //    {  offset, -offset  }   // bottom-right    
    //};
    //glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (float*)offsets);
    //int edge_kernel[9] = {
    //    -1, -1, -1,
    //    -1,  8, -1,
    //    -1, -1, -1
    //};
    //glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    //float blur_kernel[9] = {
    //    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f,
    //    2.0f / 16.0f, 4.0f / 16.0f, 2.0f / 16.0f,
    //    1.0f / 16.0f, 2.0f / 16.0f, 1.0f / 16.0f
    //};
    //glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
}

void PostProcessor::BeginRender()
{
    glBindFramebuffer(GL_FRAMEBUFFER, this->_MSFBO);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::EndRender(){
    // now resolve multisampled color-buffer into intermediate FBO to store to texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, this->_MSFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->_FBO);
    glBlitFramebuffer(0, 0, this->_width, this->_height, 0, 0, this->_width, this->_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // binds both READ and WRITE framebuffer to default framebuffer
}

void PostProcessor::Render(float time){
    // set uniforms/options
    Shader* shader = ResourceManager::GetShader("postprocess");
    shader->Use();

    //this->PostProcessingShader.SetFloat("time", time);
    //this->PostProcessingShader.SetInteger("confuse", this->Confuse);
    //this->PostProcessingShader.SetInteger("chaos", this->Chaos);
    //this->PostProcessingShader.SetInteger("shake", this->Shake);
    // render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->_texture.Bind();
    glBindVertexArray(this->_VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::_initRenderData(){
    // configure VAO/VBO
    unsigned int VBO;
    float vertices[] = {
        // pos        // tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->_VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->_VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    const Shader* shader = ResourceManager::GetShader("postprocess");
    shader->SetInteger("colorId", 3);
}