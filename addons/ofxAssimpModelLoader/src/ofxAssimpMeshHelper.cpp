//
//  ofxAssimpMeshHelper.cpp
//  Created by Lukasz Karluk on 4/12/12.
//

#include "ofxAssimpMeshHelper.h"
#include "ofxAssimpUtils.h"

ofxAssimpMeshHelper::ofxAssimpMeshHelper() {
    mesh = NULL;
    blendMode = OF_BLENDMODE_ALPHA;
    twoSided = false;
    hasChanged = false;
    validCache = false;
}

void ofxAssimpMeshHelper::addTexture(ofxAssimpTexture & aAssimpTex){
	
	if( aAssimpTex.getTextureType() == aiTextureType_DIFFUSE ){
		bool bNeedsDiffuseAdd = false;
		if( !hasTexture(aiTextureType_DIFFUSE) ){
			bNeedsDiffuseAdd = true;
		}
		
		//we do this so we can have the diffuse texture in the meshTextures vector but also accessible via the old approach .assimpTexture;
		//wouldn't need to do this if we had protected vars :P
		assimpTexture = aAssimpTex;
		if( bNeedsDiffuseAdd ){
			meshTextures.push_back(shared_ptr<ofxAssimpTexture>(&assimpTexture,[](ofxAssimpTexture*){}));
		}
	}else{
		
		auto otherTex = make_shared<ofxAssimpTexture>();
		(*otherTex.get()) = aAssimpTex;
		
		meshTextures.push_back(otherTex);
	}
	
    bool bAmbientOcclusion = false;
    bAmbientOcclusion = meshTextures.back()->getTextureType() == 17; //17 = aiTextureType_AMBIENT_OCCLUSION; //use this when we want to support newer assimp only 
    
	if( meshTextures.back()->getTextureType() == aiTextureType_EMISSIVE ){
		material.setEmissiveTexture(meshTextures.back()->getTextureRef());
	}
	else if( meshTextures.back()->getTextureType() == aiTextureType_NORMALS ){
		material.setNormalTexture(meshTextures.back()->getTextureRef());
	}
	else if( meshTextures.back()->getTextureType() == aiTextureType_LIGHTMAP || bAmbientOcclusion ){
		material.setOcclusionTexture(meshTextures.back()->getTextureRef());
	}
	else if( meshTextures.back()->getTextureType() == aiTextureType_AMBIENT ){
		material.setAmbientTexture(meshTextures.back()->getTextureRef());
	}
	else if( meshTextures.back()->getTextureType() == aiTextureType_SPECULAR ){
		material.setSpecularTexture(meshTextures.back()->getTextureRef());
	}
}

bool ofxAssimpMeshHelper::hasTexture(aiTextureType aTexType){
	if( aTexType == aiTextureType_DIFFUSE ){
		return assimpTexture.hasTexture();
	}else{
		for( auto & tex : meshTextures ){
			if( tex && tex->getTextureType() == aTexType){
				return tex->hasTexture();
			}
		}
	}
	return false;
}


ofTexture & ofxAssimpMeshHelper::getTextureRef(aiTextureType aTexType) {

	if( aTexType == aiTextureType_DIFFUSE ){
		return assimpTexture.getTextureRef();
	}else{
		for( auto & tex : meshTextures ){
			if( tex && tex->getTextureType() == aTexType){
				return tex->getTextureRef();
			}
		}
	}
	
	return assimpTexture.getTextureRef();
}
