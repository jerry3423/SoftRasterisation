#include "LeedsGL.h"
#include <math.h>

using namespace std;
LeedsGL::LeedsGL()
{
    //Initialize all variables
    inputQueue.clear();
    transformedQueue.clear();
    primitivesQueue.clear();
    clippedPrimitivesQueue.clear();
    fragmentQueue.clear();

    //TODO: Initialize all variables that should be initialized!
}

LeedsGL::~LeedsGL()
{
    //TODO: Free any resources you allocate.
}

void LeedsGL::clear(byte mask)
{
    //if depth enable
    if(mask==std::byte{3}){
        resizeBuffers(frameBuffer.width,frameBuffer.height);
    }
    //clear frame buffer color
    for(unsigned int row = 0; row < frameBuffer.height; row++){
        for(unsigned int c = 0;c < frameBuffer.width; c++){
            unsigned int index = row * frameBuffer.width + c;
            frameBuffer.block[index] = bufferClearColor;
        }
    }

    //TODO: Clear the buffer requested using the provided mask.
    //Look at the usage of this function on LeedsGLRenderWidget to
    //know what to expect of the "mask" parameter. Each bit should
    //tell which buffer to clear.
}

void LeedsGL::setUniform(const string& name,const bool value)
{
    if(name == "lightingEnabled"){
        lightingEnabled = value;
    }
    if(name == "textureModulationEnabled"){
        textureModulationEnabled = value;
    }
    if(name == "texturingEnabled"){
        texturingEnabled = value;
    }
    if(name == "UVColorDebug"){
        UVColourDebug = value;
    }
   //TODO: set an uniform bool with given name value.
   //uniform variables should decide how your shading happens
}

void LeedsGL::setUniform(const string &name, const Matrix4 &mat)
{
    if(name == "viewportMatrix"){
        viewPortMatrix = mat;
    }
    if(name == "projectionMatrix"){
        projectionMatrix = mat;
    }
    if(name == "lightMatrix"){
        lightMatrix = mat;
    }
    if(name == "modelviewMatrix"){
        modelviewMatrix = mat;
    }
    //TODO: set an uniform matrix with given name.
    //uniform variables should decide how your shading happens
}

void LeedsGL::setUniform(const string &name, const RGBAValueF &col)
{
    if(name == "lightColour"){
        lightColour = col;
    }
    if(name == "emissiveMaterial"){
        emissiveMaterial = col;
    }
    if(name == "ambientMaterial"){
        ambientMaterial = col;
    }
    if(name == "diffuseMaterial"){
        diffuseMaterial = col;
    }
    if(name == "specularMaterial"){
        specularMaterial = col;
    }
    //TODO: set an uniform colour with given name.
    //uniform variables should decide how your shading happens
}

void LeedsGL::setUniform(const std::string &name, const float val)
{
    if(name == "shininessMaterial"){
        shininessMaterial = val;
    }
    //TODO: set an uniform float with given name.
    //uniform variables should decide how your shading happens
}

void LeedsGL::setUniform(const std::string& name, const Homogeneous4& pos)
{
    if(name == "lightPosition"){
        lightPosition = pos;
    }
    //TODO: set an uniform position with given name.
    //uniform variables should decide how your shading happens
}

void LeedsGL::clearColor(const RGBAValueF &col)
{   
    bufferClearColor = col;
    //TODO: set a specific pipeline value, the color used to clear the buffer.
}

void LeedsGL::resizeBuffers(unsigned const int width, unsigned const int height)
{
    frameBuffer.Resize(width, height);
    for(unsigned int i=0;i<height;i++){
        for(unsigned int j=0;j<width;j++){
            //reset depth to a large negative number
            frameBuffer.depth.push_back(-1000.0f);
        }
    }
    //TODO: Implement how you resize your depth buffer
}

Matrix4 LeedsGLUtils::calculateViewportMatrix(float cx, float cy, float width, float height)
{
   //TODO: return a matrix that given parameters cx,cy (center of your viewport) width height
   //performs the viewport transformation. NDCS -> DCS.
    Matrix4 a;
    a.SetIdentity();
    a[0][0] = - width / 2;
    a[1][1] = - height / 2;
    a[0][3] = (cx + width) / 2;
    a[1][3] = (cy + height) / 2;
   return a;
}

Matrix4 LeedsGLUtils::calculateProjectionOrtho(float left, float right, float bottom, float top, float near, float far)
{
    //TODO: return a Ortographic projection matrix, with the parameters above.
    //right or left handedness may have effects on other parts of your code,
    //such as shading, clipping, culling, etc.
    float x_range = right - left;
    float y_range = top - bottom;
    float z_range = far - near;
    Matrix4 a;
    a.SetIdentity();
    a[0][0] = 2 / x_range;
    a[1][1] = 2 / y_range;
    a[2][2] = 1 / z_range;
    a[0][3] = - (left + right) / x_range;
    a[1][3] = - (bottom + top) / y_range;
    a[2][3] =  - (near) / z_range;
    return a;
}

Matrix4 LeedsGLUtils::calculateProjectionFrustum(float left, float right, float bottom, float top, float near, float far)
{
    //TODO: return a Perspective projection matrix, with the parameters above.
    //right or left handedness may have effects on other parts of your code,
    //such as shading, clipping, culling, etc.
    float z_range = far - near;
    Matrix4 a;
    a.SetIdentity();
    a[0][0] = 2 * near / right - left;
    a[0][2] = - (right + left) / (right - left);
    a[1][2] = - (top + bottom) / (top - bottom);
    a[1][1] = 2 * near / top - bottom;
    a[2][2] = far / z_range;
    a[2][3] = - near * far / z_range;
    a[3][2] = 1;
    a[3][3] = 0;
    return a;
}


void LeedsGL::texImage2D(RGBAImage const *textureImage)
{
    if(textureImage->width>0 && textureImage->height>0){
        enabledTexture = textureImage;
    }
    //TODO: set in your pipeline which texture should be used to render.
    //Parameter is a pointer to the texture, be aware of how it is stored, and ownership of the resources.
}

void LeedsGL::enable(const std::byte function)
{
    if(function == PERSPECTIVE){
        perspective = true;
    }
    if(function == DEPTHTEST){
        depthTestEnabled = true;
    }
    //TODO: enables a pipeline function described by the byte parameter.

}

void LeedsGL::disable(const std::byte function)
{
    if(function == PERSPECTIVE){
        perspective = false;
    }
    if(function == DEPTHTEST){
        depthTestEnabled = false;
    }
    //TODO: disables a pipeline function described by the byte parameter.

}

void LeedsGL::lineWidth(const float width)
{
    rasterizedLineWidth = width;
    //TODO: Set a variable that describes what is the width in pixels of the line to be rasterized.
}

void LeedsGL::pointSize(const float size)
{
    rasterizedPointSize = size;
    //TODO: Set a variable that describes what is the size in pixels of the point to be rasterized.
}

void LeedsGL::drawArrays(const std::vector<Homogeneous4>& vertices, const std::vector<Homogeneous4>& normals, const std::vector<Cartesian3>& textureCoordinates, const std::vector<RGBAValueF>& colors,std::byte mode)
{
    //Calls the whole pipeline, step by step.
    inputAssembly(vertices,normals,textureCoordinates,colors,inputQueue);
    transformVertices(inputQueue,transformedQueue);
    primitiveAssembly(transformedQueue,mode,primitivesQueue);
    clipAndCull(primitivesQueue,mode,clippedPrimitivesQueue);
    rasterisePrimitives(clippedPrimitivesQueue,mode,fragmentQueue);
    processFragments(fragmentQueue);

}

void LeedsGL::inputAssembly(const std::vector<Homogeneous4> &vertices, const std::vector<Homogeneous4> &normals, const std::vector<Cartesian3> &textureCoordinates, const std::vector<RGBAValueF> &colors, std::vector<InputVertex> &result)
{
    //assemble the input vectors in one vector
    result.clear();
    for(unsigned int i=0;i<vertices.size();i++){
        InputVertex temp;
        temp.vertex = vertices[i];
        //if there is no normals
        if(normals.size()!=0){
            temp.normal = normals[i];
        }
        //if there is no textures
        if(textureCoordinates.size()!=0){
            temp.textureCoordinate = textureCoordinates[i];
        }
        temp.color = colors[i];
        result.push_back(temp);
    }
    //TODO: Check how the input is passed to drawArrays.
    //This function should combine this disjoint information into a series of InputVertex to be processed
    //by the next step.
}

void LeedsGL::transformVertices(std::vector<InputVertex> &vertices, std::vector<TransformedVertex>& result)
{
    result.clear();
    for(unsigned int i=0;i<vertices.size();i++){
        TransformedVertex temp;
        //tranform the position
        temp.position = (viewPortMatrix * projectionMatrix * modelviewMatrix * vertices[i].vertex).Point();
        temp.color = vertices[i].color;
        temp.normal = vertices[i].normal.Point();
        temp.textureCoordinate = vertices[i].textureCoordinate;
        //also store the position used in light
        temp.v_pos = modelviewMatrix * vertices[i].vertex;
        result.push_back(temp);
    }
   //TODO: Transform the input vertices using the matrices set in LeedsGLRenderWidget.
   //Also pass all the necessary information to the next steps of the pipeline.
   //You should check the slides to decide which is the appropriate coordinate system to transform them to.
}

void LeedsGL::primitiveAssembly(std::vector<TransformedVertex> &vertices,std::byte mode, std::vector<Primitive>& result)
{
    result.clear();
    //if points, assemble primitives using point
    if(mode == POINTS){
        Primitive temp;
        for(unsigned int i=0;i<vertices.size();i++){
            temp.transformedVertices.push_back(vertices[i]);
            result.push_back(temp);
            temp.transformedVertices.clear();
        }
    }
    //if lines, assemble primitives using two points in a line
    if(mode == LINES){
        Primitive temp;
        for(unsigned int i=0;i<vertices.size()-1;i+=2){
            temp.transformedVertices.push_back(vertices[i]);
            temp.transformedVertices.push_back(vertices[i+1]);
            result.push_back(temp);
            temp.transformedVertices.clear();
        }
    }
    //if triangles, assemble primitives using three vertices in a triangle
    if(mode == TRIANGLES){
        Primitive temp;
        for(unsigned int i=0;i<vertices.size()-2;i+=3){
            temp.transformedVertices.push_back(vertices[i]);
            temp.transformedVertices.push_back(vertices[i+1]);
            temp.transformedVertices.push_back(vertices[i+2]);
            result.push_back(temp);
            temp.transformedVertices.clear();
        }
    }
   //TODO: Assemble the vertices into a primitive according to the selected mode.
}

void LeedsGL::clipAndCull(std::vector<Primitive>& primitives,std::byte mode, std::vector<Primitive>& result)
{
    //return the same primitives
    result.clear();
    if(mode == POINTS){
        for(unsigned int i=0;i<primitives.size();i++){
            result.push_back(primitives[i]);
        }
    }
    if(mode == LINES){
        for(unsigned int i=0;i<primitives.size();i++){
            result.push_back(primitives[i]);
        }
    }
    if(mode == TRIANGLES){
        for(unsigned int i=0;i<primitives.size();i++){
            result.push_back(primitives[i]);
        }
    }
    //TODO: Implement clipping and culling. Should have a different behavior for each type of primitive.
    //Pay attention to what type of projection you are using, as your clipping planes will be different.
    //If you choose to skip this step as it is one of your last tasks, just return all the same primitives.
}

void LeedsGL::rasterisePrimitives(std::vector<Primitive> &primitives, std::byte mode, std::vector <Fragment>& results)
{
    //according to different modes, use differnet functions
    results.clear();
    if(mode == POINTS){
        for(unsigned int i=0;i<primitives.size();i++){
            rasterisePoint(primitives[i],results);
        }
    }
    if(mode == LINES){
        for(unsigned int i=0;i<primitives.size();i++){
            rasteriseLine(primitives[i],results);
        }
    }
    if(mode == TRIANGLES){
        for(unsigned int i=0;i<primitives.size();i++){
            rasteriseTriangle(primitives[i],results);
        }
    }
    //TODO: Generate a list of fragments according to what mode is chosen. Should call the "rasterise X" functions
}

void LeedsGL::rasterisePoint(const Primitive &point,std::vector<Fragment>& output)
{
    for(unsigned int j=0;j<rasterizedPointSize;j++){
        for(unsigned int k=0;k<rasterizedPointSize;k++){
            Fragment temp;
            //store fragment from bottom left to top right
            unsigned int r = unsigned(int(point.transformedVertices[0].position.y - rasterizedPointSize / 2)) + j;
            unsigned int c = unsigned(int(point.transformedVertices[0].position.x - rasterizedPointSize / 2)) + k;
            //give parameters to fragment
            temp.row = int(r);
            temp.col = int(c);
            temp.bcColor = point.transformedVertices[0].color;
            temp.depth = point.transformedVertices[0].position.z;
            output.push_back(temp);
        }
    }
    //TODO: Rasterise a point, according to the pointSize.
}

void LeedsGL::rasteriseLine(const Primitive &line, std::vector<Fragment> &output)
{
    //set two points in a line
    Cartesian3 p = Cartesian3(line.transformedVertices[0].position.x,line.transformedVertices[0].position.y,line.transformedVertices[0].position.z);
    Cartesian3 q = Cartesian3(line.transformedVertices[1].position.x,line.transformedVertices[1].position.y,line.transformedVertices[1].position.z);
    Cartesian3 temp;
    RGBAValueF color = RGBAValueF(0,0,0,1);
    //use Interpolation to get color and row and col
    for(float t = 0.0f;t<=1.0f;t+=0.001f){
        temp = p + (q - p) * t;
        color = line.transformedVertices[0].color + t * (line.transformedVertices[1].color);
        for(unsigned int i=0;i<rasterizedLineWidth;i++){
            Fragment f;
            //give parameters to fragment
            f.row = int(temp.y);
            f.col = int(temp.x) + int(i) - int(rasterizedLineWidth / 2);
            f.depth = temp.z;
            f.bcColor = color;
            output.push_back(f);
        }
    }
   //TODO: Rasterise a line, according to the linewidth
}


float LeedsGLUtils::distancePointLine(Cartesian3 r, Cartesian3 n, Cartesian3 p)
{
    //assumes n is normalized
       return n.dot(r) - n.dot(p);
}
void LeedsGL::rasteriseTriangle(const Primitive &triangle, std::vector<Fragment> &output)
{
    TransformedVertex vertex0 = triangle.transformedVertices[0];
    TransformedVertex vertex1 = triangle.transformedVertices[1];
    TransformedVertex vertex2 = triangle.transformedVertices[2];

    // compute a bounding box that starts inverted to frame size
    // clipping will happen in the raster loop proper
    float minX = frameBuffer.width, maxX = 0.0;
    float minY = frameBuffer.height, maxY = 0.0;

    // test against all vertices
    if (vertex0.position.x < minX) minX = vertex0.position.x;
    if (vertex0.position.x > maxX) maxX = vertex0.position.x;
    if (vertex0.position.y < minY) minY = vertex0.position.y;
    if (vertex0.position.y > maxY) maxY = vertex0.position.y;

    if (vertex1.position.x < minX) minX = vertex1.position.x;
    if (vertex1.position.x > maxX) maxX = vertex1.position.x;
    if (vertex1.position.y < minY) minY = vertex1.position.y;
    if (vertex1.position.y > maxY) maxY = vertex1.position.y;

    if (vertex2.position.x < minX) minX = vertex2.position.x;
    if (vertex2.position.x > maxX) maxX = vertex2.position.x;
    if (vertex2.position.y < minY) minY = vertex2.position.y;
    if (vertex2.position.y > maxY) maxY = vertex2.position.y;

    Cartesian3 v0 = Cartesian3(vertex0.position.x,vertex0.position.y,0);
    Cartesian3 v1 = Cartesian3(vertex1.position.x,vertex1.position.y,0);
    Cartesian3 v2 = Cartesian3(vertex2.position.x,vertex2.position.y,0);

    Cartesian3 v0v1 = v1-v0;
   Cartesian3 n_v0v1 = Cartesian3(-v0v1.y,v0v1.x,0);
   Cartesian3 v1v2 = v2-v1;
   Cartesian3 n_v1v2 = Cartesian3(-v1v2.y,v1v2.x,0);
   Cartesian3 v2v0 = v0-v2;
   Cartesian3 n_v2v0 = Cartesian3(-v2v0.y,v2v0.x,0);

   float dAlpha =  LeedsGLUtils::distancePointLine(v0, n_v1v2,v1);
   float dBeta = LeedsGLUtils::distancePointLine(v1, n_v2v0,v2);
   float dGamma = LeedsGLUtils::distancePointLine(v2,n_v0v1,v0);

   if (abs(dAlpha-0)<std::numeric_limits<float>::epsilon() ||
       abs(dBeta-0)<std::numeric_limits<float>::epsilon() ||
       abs(dGamma-0)<std::numeric_limits<float>::epsilon())
        return;

    // create a fragment for reuse
    Fragment rasterFragment;

    // loop through the pixels in the bounding box
    for (rasterFragment.row = int(minY); rasterFragment.row <= maxY; rasterFragment.row++)
    { // per row
        // this is here so that clipping works correctly
        if (rasterFragment.row < 0) continue;
        if (rasterFragment.row >= int(frameBuffer.height)) continue;
        for (rasterFragment.col = int(minX); rasterFragment.col <= maxX; rasterFragment.col++)
        { // per pixel
            // this is also for correct clipping
            if (rasterFragment.col < 0) continue;
            if (rasterFragment.col >= int(frameBuffer.width)) continue;

            // the pixel in cartesian format
            Cartesian3 pixel(rasterFragment.col+0.5f, rasterFragment.row+0.5f, 0.0f);

            // right - we have a pixel inside the frame buffer AND the bounding box
            // note we *COULD* compute gamma = 1.0 - alpha - beta instead
            float alpha = LeedsGLUtils::distancePointLine(pixel,n_v1v2,v1) / dAlpha;
            float beta = LeedsGLUtils::distancePointLine(pixel,n_v2v0,v2)/dBeta;
            float gamma = LeedsGLUtils::distancePointLine(pixel,n_v0v1,v0)/dGamma;

            // now perform the half-plane test
            if ((alpha < 0.0f) || (beta < 0.0f) || (gamma < 0.0f))
                continue;
            //store row, col, color in fragment
            rasterFragment.row = int(pixel.y);
            rasterFragment.col = int(pixel.x);

            //perspective correct and baricentric interpolate
            float z = 1 / ((alpha / vertex0.v_pos.z) + (beta / vertex1.v_pos.z) + (gamma / vertex2.v_pos.z));
            RGBAValueF color = alpha * vertex0.color + beta * vertex1.color + gamma * vertex2.color;
            Cartesian3 uvw = z * ((alpha / vertex0.v_pos.z) * vertex0.textureCoordinate + (beta / vertex1.v_pos.z) * vertex1.textureCoordinate + (gamma / vertex2.v_pos.z) * vertex2.textureCoordinate);

            //used in depth test
            rasterFragment.depth = z;

            //calculate baricentric postion and normal in vcs, used in light
            Homogeneous4 bcv = alpha * vertex0.v_pos + beta * vertex1.v_pos + gamma * vertex2.v_pos;
            Homogeneous4 bcn = alpha * vertex0.normal + beta * vertex1.normal + gamma * vertex2.normal;

            //"RGB:=UVW"
            if(UVColourDebug){
                rasterFragment.bcColor = RGBAValueF(uvw.x,uvw.y,uvw.z,1);
            }
            else{
                rasterFragment.bcColor = color;
            }

            //light
            if(lightingEnabled){
                color = CalculateLighting(bcn,bcv,emissiveMaterial,ambientMaterial,diffuseMaterial,specularMaterial,shininessMaterial);
                rasterFragment.bcColor = color;
            }

            //texture
            if(texturingEnabled){
                rasterFragment.tex_row = uvw.x * enabledTexture->width;
                rasterFragment.tex_col = uvw.y * enabledTexture->height;
            }

            output.push_back(rasterFragment);
           //TODO: use the computed baricentric coordinates to interpolate all of the necessary properties for rendering.
           //Be aware of making sure they are perspective correct.

        } // per pixel
    } // per row
}

void LeedsGL::processFragments(std::vector<Fragment> &fragments)
{
    for(unsigned int i=0;i<fragments.size();i++){
        //find pixel index
        unsigned int index = unsigned(fragments[i].row) * frameBuffer.width + unsigned(fragments[i].col);

        //depth test
        if(depthTestEnabled){
            //if the fragment depth less than framebuffer, continue
            if(fragments[i].depth < frameBuffer.depth[index]){
                continue;
            }
            else{
                frameBuffer.depth[index] = fragments[i].depth;
                frameBuffer.block[index] = fragments[i].bcColor;
            }
        }
        else{
            frameBuffer.block[index] = fragments[i].bcColor;
        }

        //if texture enable, give color from enabledTexture to framebuffer
        if(texturingEnabled){
            //bilinear filtering
            int tex_i = int(fragments[i].tex_row);
            int tex_j = int(fragments[i].tex_col);

            float s_parm = fragments[i].tex_row - tex_i;
            float t_parm = fragments[i].tex_col - tex_j;
            //grab four nearest colors
            RGBAValue color00 = enabledTexture->block[unsigned(tex_i) * enabledTexture->width + unsigned(tex_j)];
            RGBAValue color01 = enabledTexture->block[unsigned(tex_i) * enabledTexture->width + unsigned(tex_j+1)];
            RGBAValue color10 = enabledTexture->block[unsigned(tex_i+1) * enabledTexture->width + unsigned(tex_j)];
            RGBAValue color11 = enabledTexture->block[unsigned(tex_i+1) * enabledTexture->width + unsigned(tex_j+1)];
            //compute colors on edges
            RGBAValue color0 = color00 + t_parm * (color01 + (-1) * color00);
            RGBAValue color1 = color10 + t_parm * (color11 + (-1) * color10);
            //compute tex color
            RGBAValue tex_color = color1 + s_parm * (color1 + (-1) * color0);
            tex_color = enabledTexture->block[unsigned(tex_i) * enabledTexture->width + unsigned(tex_j)];
            //if modulation enable, mix the light color
            if(textureModulationEnabled){
                frameBuffer.block[index] = frameBuffer.block[index].modulate(tex_color);
            }
            else{
                //if not ,give texture color to frame
                frameBuffer.block[index] =tex_color;
            }
        }
    }
    fragments.clear();
    frameBuffer.depth.clear();
   //TODO: Process all of the fragments, shading according to the uniform properties.
   //Depth test should go here. We don't explicitly have a pre or post fragment stage.
   //Consider the "shading" as the fragment stage. Decide if the depth test should go before or after, and justify.
}

RGBAValueF LeedsGL::CalculateLighting(const Homogeneous4& n_vcs, const Homogeneous4& v_vcs,const RGBAValueF& em, const RGBAValueF& am, const RGBAValueF& diff, const RGBAValueF& spec, float shin)
{
    if((n_vcs.x == 0.0f && n_vcs.y == 0.0f && n_vcs.z == 0.0f)) // we shouldn't try shading if there are no normals
    return RGBAValueF();

    Cartesian3 lightVector;
    Cartesian3 unitNormal = n_vcs.Vector().unit();
    //Directional Light

    Homogeneous4 lp = lightMatrix * lightPosition;

    if(abs(lp.w - 0) < std::numeric_limits<float>::epsilon())
        lightVector = lp.Vector().unit();
    else //point light
        lightVector = (lp - v_vcs).Vector().unit();
    Cartesian3 eyeVector = perspective? -1*v_vcs.Point(): Cartesian3(0,0,1);
    Cartesian3 bisector = (lightVector + eyeVector).unit();

    RGBAValueF emissive = em;
    RGBAValueF ambient =  am.modulate(lightColour);

    float dDot = unitNormal.dot(lightVector);
    dDot = dDot <0? 0: dDot;

    RGBAValueF diffuse = dDot * diff.modulate(lightColour);

    float sDot = unitNormal.dot(bisector);
    sDot = sDot <0? 0:sDot;
    sDot = pow(sDot,shin);
    //sDot = ((shin+2)/8.0f)*sDot*dDot;
    sDot = dDot>0? sDot : 0;
    sDot = sDot* dDot *(shin+2)/2*float(M_PI);

    Cartesian3 fs = Cartesian3(spec.red,spec.green,spec.blue);
    Cartesian3 air = Cartesian3(1,1,1);
    Cartesian3 a = (air - fs);
    Cartesian3 b = (air+fs);
    Cartesian3 r0 = Cartesian3(a.x/b.x,a.y/b.y,a.z/b.z);
    r0 = Cartesian3(r0.x*r0.x,r0.y*r0.y,r0.z*r0.z);
    Cartesian3 rschlick = r0 + (air-r0) * powf((1-bisector.dot(lightVector)),5);
    RGBAValueF updatedSpecular = RGBAValueF(rschlick.x,rschlick.y,rschlick.z,1);
    RGBAValueF specular =sDot * updatedSpecular.modulate(lightColour);

    return emissive + ambient + diffuse + specular;
}
