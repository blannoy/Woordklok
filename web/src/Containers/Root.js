import React, {useState, useEffect,useReducer} from "react";
import Main from "./Main";


import {configContext, queryProviderContext, clockFaceContext, requestProviderContext,clockContext} from "../Context/Context";
import {mapClockFace} from "../Utils/ClockFaceMapper";
import useRequest, {requestReducer,defaultRequest } from "../Context/Reducer";
import {useGetClockfaceQuery, useGetConfigQuery} from "../Components/ClockAPI";

export default function Root() {
    const [fullConfig,setFullConfig]=useState(undefined)
    const {data:configData, error: configError, isLoading: configIsLoading } = useGetConfigQuery();
    const [error,setError]=useState({currentUrl:"",object:{}});

    useEffect(() => {
        if (configData !== undefined) {
            setFullConfig({...configData});
        }
    }
    , [configData]);
    useEffect(() => {
      let err = null;
        if (configError !== undefined) {
            err = configError;
        }
        setError({ currentUrl: window.location.href, object: {...err}});

    }, [configError]);


    return (
    <clockContext.Provider value={[fullConfig,setFullConfig]}>
                <Main />
            {(Object.keys(error.object).length >0)&&
            <div>
             {(error.currentUrl === window.location.href) &&
             (<pre>
              {error.object.message}<br/>
          {(error.object.response?JSON.stringify(error.object.response.data):"")}</pre>) }
            </div>
            }
      </clockContext.Provider>

        /* <configContext.Provider value={[initialConfig,setInitialConfig]}>
          <clockFaceContext.Provider value={[clockFace,setClockFace]}>
          <queryProviderContext.Provider value={[requestState,dispatchRequest]}>
          <requestProviderContext.Provider value={[responseObject, setResponseObject]}>
            <Main />
            {(Object.keys(error.object).length >0)&&
            <div>
             {(error.currentUrl === window.location.href) &&
             (<pre>
              {error.object.message}<br/>
          {(error.object.response?JSON.stringify(error.object.response.data):"")}</pre>) }
            </div>
            }
            </requestProviderContext.Provider>
            </queryProviderContext.Provider>
            </clockFaceContext.Provider>
        </configContext.Provider> */
    );
}
