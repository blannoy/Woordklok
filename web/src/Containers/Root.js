import React, {useState, useEffect,useReducer} from "react";
import Main from "./Main";
import {configContext, queryProviderContext, clockFaceContext, requestProviderContext} from "../Context/Context";
import {mapClockFace} from "../Utils/ClockFaceMapper";
import useRequest, {requestReducer,defaultRequest } from "../Context/Reducer";

export default function Root() {
    const [initialConfig,setInitialConfig]=useState(null);
    const [clockFace,setClockFace]=useState(null);
    const [error,setError]=useState(null);
    const [response, err, runRequest]  = useRequest();
    const [requestState, dispatchRequest] = useReducer(requestReducer, defaultRequest)
    const [responseObject, setResponseObject] = useState(null);

    useEffect(() => {
      if (requestState.url !== ""){
        runRequest(requestState);
      }
  }, [requestState]);
  
  useEffect(() => {
    if (initialConfig === null && response === null) {
      dispatchRequest({ type: 'LOADCONFIG' });
    } else if (response !== null) {
      switch (response.config.url) {
        case "/config":
          setInitialConfig(response.data);
          setClockFace(mapClockFace(response.data.clockface));
          dispatchRequest({ type: 'STATUS' });
          break;
          case "/status":
            setResponseObject(response.data);
            break;
        default:
          if (response.config.url !== "/calibrateLdr") {
           if (response.data!==undefined){
             setInitialConfig(response.data);
            }
          }
          break;
      }

    }
    if (err) {
      setError(err);
    }
  }, [response, err])

    return (
        <configContext.Provider value={[initialConfig,setInitialConfig]}>
          <clockFaceContext.Provider value={[clockFace,setClockFace]}>
          <queryProviderContext.Provider value={[requestState,dispatchRequest]}>
          <requestProviderContext.Provider value={[responseObject, setResponseObject]}>
            <Main />
            <div>
            <pre>{error}</pre>
            </div>
            </requestProviderContext.Provider>
            </queryProviderContext.Provider>
            </clockFaceContext.Provider>
        </configContext.Provider>
    );
}
