/***
 * Copyright (C) Rodolfo Herrera Hernandez. All rights reserved.
 * Licensed under the MIT license. See LICENSE file in the project root 
 * for full license information.
 *
 * =+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 *
 * Module.hxx: Javascript modules.
 *
 * This file allows the execution of Zenda scripts and allows the import of modules.
 *
 * For related information - https://github.com/rodiihernandezz/ZendaJS/
 *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
 ****/

static MaybeLocal<Module> LoadModule(char Code[], char Name[], Local<Context> MContext);
static Local<Module> CheckModule(MaybeLocal<Module> MaybeModule, Local<Context> MContext);
static Local<Value> ExecuteModule(Local<Module> LModule, Local<Context> MContext, bool NsObject);
static inline MaybeLocal<Module> CallResolve(Local<Context> MContext, Local<String> Specifier, Local<Module> Referrer);
static MaybeLocal<Promise> CallDynamic(Local<Context> MContext, Local<ScriptOrModule> Referrer, Local<String> Specifier);

static MaybeLocal<Module> LoadModule(char Code[], char Name[], Local<Context> MContext){
    Local<String> VMCode = String::NewFromUtf8(MContext->GetIsolate(), Code).ToLocalChecked();
    ScriptOrigin Origin(
        String::NewFromUtf8(MContext->GetIsolate(), Name).ToLocalChecked(),
        Integer::New(MContext->GetIsolate(), 0),
        Integer::New(MContext->GetIsolate(), 0),
        False(MContext->GetIsolate()),
        Local<Integer>(),
        Local<Value>(),
        False(MContext->GetIsolate()),
        False(MContext->GetIsolate()),
        True(MContext->GetIsolate())
    );
    Context::Scope ContextScope(MContext);
    ScriptCompiler::Source LSource(VMCode, Origin);
    MaybeLocal<Module> LModule;
    LModule = ScriptCompiler::CompileModule(MContext->GetIsolate(), &LSource);
    return LModule;
}

static Local<Module> CheckModule(MaybeLocal<Module> MaybeModule, Local<Context> MContext){
    Local<Module> LModule;
    TryCatch try_catch(MContext->GetIsolate());
    if(!MaybeModule.ToLocal(&LModule)){
        assert(try_catch.HasCaught());
        ReportException(&try_catch);
        exit(EXIT_FAILURE);
    }
    Maybe<bool> Result = LModule->InstantiateModule(MContext, CallResolve);
    if(Result.IsNothing()){
        assert(try_catch.HasCaught());
        ReportException(&try_catch);
        cout << "An error occurred while loading the modules." << endl;
        exit(EXIT_FAILURE);
    }
    assert(!try_catch.HasCaught());
    return LModule;
}

static Local<Value> ExecuteModule(Local<Module> LModule, Local<Context> MContext, bool NsObject = false){
    Local<Value> ReturnValue;
    TryCatch try_catch(MContext->GetIsolate());
    if(!LModule->Evaluate(MContext).ToLocal(&ReturnValue)){
        assert(try_catch.HasCaught());
        ReportException(&try_catch);
        exit(EXIT_FAILURE);
    }
    assert(!try_catch.HasCaught());
    if(NsObject)
        return LModule->GetModuleNamespace();
    else
        return ReturnValue;
}

static inline MaybeLocal<Module> CallResolve(Local<Context> MContext, Local<String> Specifier, Local<Module> Referrer){
    String::Utf8Value Str(MContext->GetIsolate(), Specifier);
    if(FSFileExists(*Str))
        return LoadModule(strdup(FSReadFile(*Str)), *Str, MContext);
    else if(FSFileExists(PackagesFolder + *Str)){
        string MaybePackageModule = PackagesFolder + *Str;
        return LoadModule(strdup(FSReadFile(MaybePackageModule.c_str())), strdup(MaybePackageModule.c_str()), MContext);
    }else{
        string MaybeSourceCodeModule = ZendaSourceCodeLocation() + "/" + string(ToCString(Str));
        return LoadModule(strdup(FSReadFile(MaybeSourceCodeModule.c_str())), strdup(MaybeSourceCodeModule.c_str()), MContext);
    }
}

static MaybeLocal<Promise> CallDynamic(Local<Context> MContext, Local<ScriptOrModule> Referrer, Local<String> Specifier){
    Local<Promise::Resolver> Resolver = Promise::Resolver::New(MContext).ToLocalChecked();
    MaybeLocal<Promise> LPromise(Resolver->GetPromise());
    String::Utf8Value Name(MContext->GetIsolate(), Specifier);
    Local<Module> LModule = CheckModule(LoadModule(strdup(FSReadFile(*Name)), *Name, MContext), MContext);
    Local<Value> ReturnValue = ExecuteModule(LModule, MContext, true);
    Maybe<bool> MResolve = Resolver->Resolve(MContext, ReturnValue);
    return LPromise;
}