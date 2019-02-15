# ProceduralnaAnimacija

Z delom začnemo najprej s prenosom celotnega repozitorija. To lahko storimo z orodjem git ali pa ročno prenesemo arhivsko kopijo repozitorija. 
V datotekah je priložen Visual Studio projekt. Če zahtevanega urejevalnika nimamo, lahko projekt po datotekah odpremo v drugih urejevalnikih in 
v njem povežemo vse zahtevane datoteke, ki jih lahko najdemo v mapi "Libraries, DLLs, includes". Na lokacijo poti, kjer nam prevajalnik ustvari 
datoteko za zaganjanje aplikacije dodamo tudi vse datoteke z dinamičnimi knjižnicami (DLL). 

Ko nam to uspe in aplikacijo lahko zgradimo in zaženemo, lahko pogledamo še datoteke, povezane z rezultati diplomske naloge.
V mapi z izvorno kodo lahko najdemo naslednje relevantne datoteke:
- Animation/InverseKinematics/FABRIK (Skrbi za računanje inverzne kinematike)
- Animation/AnimationBoneTree (Vsebuje hierarhijo skeleta)
- Animation/AnimationSkeleton (Vsebuje vse podatke o kosteh skeleta)
- Animation/SkeletalAnimationAction (Vsebuje zaporedje animacijskih poz posameznih akcij)
- Animation/SkeletalAnimationInterpolator (Skrbi za interpoliranje med animacijskimi pozami)
- Animation/SkeletalAnimationKeyframe (Hrani posamezne animacijske poze)
- Animation/SkeletalAnimationPlayer (Upravlja s animacijsko časovnico)
- Animation/SkeletalAnimationPlayer (Shranjuje lokalno transformacijo za vsak sklep animacijske poze)
- Controller/NearCollisionGrabber (Skrbi za iskanje bližnjih kolizijskih teles, ki se jih lahko dotaknemo)
- Component/SkeletalAnimationComponent (Vsebuje vse podatke o animacijah nekega modela)
- System/SkeletalANimationRendererSystem (Skrbi za upravljanje s komponentami skeletalne animacije in njihov izris)