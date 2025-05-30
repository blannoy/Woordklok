import {useState, useEffect} from "react";
import Main from "./Main";


import {clockContext} from "../Context/Context";
import {useGetConfigQuery} from "../Components/ClockAPI";

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
    );
}
