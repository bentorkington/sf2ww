//
//  Shader.fsh
//  iMustard
//
//  Created by Ben on 17/03/14.
//
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
